//
// Created by ASUS on 30.12.2022.
//

#include <iostream>
#include <Application.h>
#include <MousePositionProviderImpl.h>
#include <Label.h>
#include <Message.h>
#include <GraphHelpers.h>
#include <Arrow.h>
#include <ColorHelpers.h>
#include "GuiComponent.hpp"
#include <ViewPositionProviderImpl.hpp>
#include <imgui.h>
#include <optional>
#include <imgui-SFML.h>
#include <Voyager.h>
#include <fmt/core.h>

Application::Application(int width, int height, const std::string &title) :
        window(sf::VideoMode(width, height), title, sf::Style::Default),
        bgColor(sf::Color::White),
        viewMoveSpeed(200.f),
        viewMouseScrollSpeed(4.f),
        viewRotationSpeed(4.f),
        pathColor(sf::Color::Green),
        defaultColor(sf::Color::Black) {
    ImGui::SFML::Init(window);
    this->entityBounder = std::make_shared<EntityBounder>(this);
    this->mouseEventDispatcher = std::make_shared<EntityEventDispatcherImpl>();
    this->guiEventDispatcher = std::make_shared<EntityEventDispatcherImpl>();
    this->arrowHolder = std::make_shared<EntityEventDispatcherImpl>();
    this->window.setVerticalSyncEnabled(true);
    this->view = this->window.getDefaultView();

    this->gui_view = this->window.getDefaultView();
    this->mppGUI = std::make_shared<MousePositionProviderImpl>(&this->window, &this->gui_view);
    this->mppWorldPos = std::make_shared<MousePositionProviderImpl>(&this->window, &this->view);
    if (!this->font.loadFromFile("../res/Montserrat_Regular.ttf")) {
        std::cerr << "ERROR LOADING FONT" << std::endl;
    };
    this->eventDispatcher.addListenerOnEvent(sf::Event::Resized,
                                             [this](const sf::Event &event, const sf::Time &dt) {
                                                 this->gui_view.setSize((float) event.size.width,
                                                                        (float) event.size.height);
                                                 this->gui_view.setCenter((float) event.size.width / 2,
                                                                          (float) event.size.height / 2);
                                                 this->view.setSize((float) event.size.width,
                                                                    (float) event.size.height);
                                                 this->view.setCenter((float) event.size.width / 2,
                                                                      (float) event.size.height / 2);

                                             });


    label = std::make_shared<Label>(this->font, "Enter Vertex Name", this->gui_view);
    label->setInViewPositionFactors(0.5, 0);
    label->getSignal(signals::onEndEditingText).addSlot(
            std::bind(&Application::onLabelTextEntered, this, std::placeholders::_1));
    this->guiEventDispatcher->addEntity(label);

    this->eventDispatcher.addListenerOnKeyPressedEvent(sf::Keyboard::Space, [this](const sf::Time &dt) {
        this->colorizeVertices();
    });


    this->mouseEventDispatcher->setMousePositionProvider(mppWorldPos);

    this->arrowHolder->setMousePositionProvider(mppWorldPos);

    this->guiEventDispatcher->setMousePositionProvider(mppGUI);

    auto buttonColor = sf::Color(238, 238, 228, 255);
    auto onClickedButtonColor = sf::Color(238, 238, 228, 128);
    this->eventDispatcher.addListenerOnKey(sf::Keyboard::D, [this](const sf::Time &dt) {
        this->view.move(sf::Vector2f(this->viewMoveSpeed, 0.f) * dt.asSeconds());
    });
    this->eventDispatcher.addListenerOnKey(sf::Keyboard::W, [this](const sf::Time &dt) {
        this->view.move(sf::Vector2f(0.f, -this->viewMoveSpeed) * dt.asSeconds());
    });
    this->eventDispatcher.addListenerOnKey(sf::Keyboard::S, [this](const sf::Time &dt) {
        this->view.move(sf::Vector2f(0.f, this->viewMoveSpeed) * dt.asSeconds());
    });
    this->eventDispatcher.addListenerOnKey(sf::Keyboard::A, [this](const sf::Time &dt) {
        this->view.move(sf::Vector2f(-this->viewMoveSpeed, 0.f) * dt.asSeconds());
    });
    this->eventDispatcher.addListenerOnKey(sf::Keyboard::Q, [this](const sf::Time &dt) {
        this->view.rotate(this->viewRotationSpeed * dt.asSeconds());
    });
    this->eventDispatcher.addListenerOnKey(sf::Keyboard::E, [this](const sf::Time &dt) {
        this->view.rotate(-this->viewRotationSpeed * dt.asSeconds());
    });
    this->eventDispatcher.addListenerOnEvent(sf::Event::MouseWheelScrolled,
                                             [this](const sf::Event &event, const sf::Time &dt) {
                                                 auto delta = event.mouseWheelScroll.delta;
                                                 if (delta > 0) {
                                                     this->view.zoom(1 + this->viewMouseScrollSpeed * dt.asSeconds());
                                                 } else {
                                                     this->view.zoom(1 - this->viewMouseScrollSpeed * dt.asSeconds());
                                                 }
                                             });
}

void Application::run() {
    sf::Event event{};
    sf::Clock clock{};
    sf::Time dt{};
    while (this->window.isOpen()) {
        dt = clock.restart();
        this->processEvents(event, dt);
        this->update(dt);
        this->render(dt);
    }

}

void Application::colorizeVertices() const {
    auto components = GraphHelpers::BoundComponents(graph);
    auto step = 1.f / (float) components.size();
    auto start = 0.f;
    for (const auto &component: components) {
        for (const auto &vertexName: component) {
            auto f_vertex = dynamic_cast<VertexEntity *>(mouseEventDispatcher->getEntityByName(vertexName));
            if (f_vertex) {
                f_vertex->setDefaultColor(
                        ColorHelpers::interpolate(sf::Color::Red, sf::Color(60, 0, 100), start, ColorHelpers::linear));
            }
        }
        start += step;
    }
}


void Application::processEvents(sf::Event &event, const sf::Time &dt) {
    while (this->window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            this->window.close();
        }
        ImGui::SFML::ProcessEvent(window, event);
        this->eventDispatcher.handleEvent(event, dt);
        this->mouseEventDispatcher->handleEvent(event);
        this->arrowHolder->handleEvent(event);
        this->guiEventDispatcher->handleEvent(event);
    };
    this->eventDispatcher.handleInput(dt);

}

void Application::update(const sf::Time &dt) {
    ImGui::SFML::Update(window, dt);
    this->mouseEventDispatcher->update(dt.asSeconds());
    this->arrowHolder->update(dt.asSeconds());
    this->guiEventDispatcher->update(dt.asSeconds());


}

void Application::render(const sf::Time &dt) {
    this->window.clear(this->bgColor);
    this->window.setView(this->view);
    this->window.draw(*this->mouseEventDispatcher);

    this->window.draw(*this->arrowHolder);

    this->window.setView(this->gui_view);
    this->window.draw(*this->guiEventDispatcher);

    imguiWindow();
    ImGui::SFML::Render(window);


    this->window.display();
}

void Application::imguiWindow() {
    ImGui::Begin("Functions");
    ImGui::StyleColorsDark();
    static bool showApplicationProperties = false;
    static const char *startDijkstraVertexName = nullptr;
    static const char *startPrimaVertexName = nullptr;
    static const char *startVoyageurVertexName = nullptr;
#pragma region createVertex
    static char buf[255];
    ImGui::InputText("- vertex name", buf, 255);
    if (ImGui::Button("Create vertex")) {
        this->createVertexByName(buf);
    }

#pragma region VertexList
    ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.2f);
    if (ImGui::BeginCombo("Start Vertex Name", startVoyageurVertexName)) {
        for (auto &e: this->mouseEventDispatcher->getEntities()) {
            bool is_selected = (startVoyageurVertexName == e->getName().c_str());
            if (ImGui::Selectable(e->getName().c_str(), is_selected)) {
                startVoyageurVertexName = e->getName().c_str();
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
#pragma endregion

    ImGui::SameLine();

#pragma region Commis-voyageur
    if (ImGui::Button("Voyageur")) {
        if (startVoyageurVertexName) {

            if (this->graph.getVerticesCount() <= 1) {
                this->createMessage("Cannot solve task", 0.5f);

            } else {
                auto tmp = this->graph.getIncidentMatrix();
                auto index = 0;
                for (index = 0; index < tmp.second.size() && !(tmp.second[index] == startVoyageurVertexName); ++index);
                auto &matrix = tmp.first;
                auto v = Voyager(matrix);
                v.solveTask(index);
                auto res = v.getRecord();
                if (res == std::numeric_limits<int>::max()) {
                    this->createMessage("Cannot solve task", 0.5f);
                } else {
                    auto path = v.getShortest();
                    std::vector<std::string> pathRes(path.size());
                    std::string stringRes;
                    stringRes.reserve(pathRes.size() * 10);
                    int c = 0;
                    for (auto &index: path) {
                        pathRes[c++] = tmp.second[index];
                    }
                    for (auto &vertex: pathRes) {
                        stringRes.append(vertex + " ");
                    }
                    this->createMessage(fmt::format("len: {}\n path: {}", v.getRecord(), stringRes), 5.f);
                    c = 1;
                    for (const auto &v: pathRes) {
                        if (c == pathRes.size()) {
                            break;
                        }
                        auto e = this->mouseEventDispatcher->getEntityByName(v);
                        auto vertexEntity = dynamic_cast<VertexEntity *>(e);
                        if (vertexEntity) {
                            vertexEntity->setAdditionalLabelString(std::to_string(c++));
                            vertexEntity->setShowAdditionalLabel(true);
                        }
                    }
                }
            }
        }
        else {
            this->createMessage("Select startVertex",1.f);
        }
    }
#pragma endregion
#pragma endregion
#pragma region top-sort
    if (ImGui::Button("Topological sort")) {
        onSortBtnClicked(nullptr);
    }
#pragma endregion
#pragma region DijkstraButton
    static bool flag = false;
    static auto dijkstraResult = std::optional<std::map<std::string, int>>(std::nullopt);
    if (ImGui::Button("Dijkstra")) {
        flag ^= true;
        if (flag) {
            if (startDijkstraVertexName != nullptr) {
                dijkstraResult = GraphHelpers::Dijkstra(this->graph, startDijkstraVertexName);
            }
        }
    }
    ImGui::SameLine();
#pragma endregion
#pragma region VertexList
    ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.2f);
    if (ImGui::BeginCombo("Start Vertex Name", startDijkstraVertexName)) {
        for (auto &e: this->mouseEventDispatcher->getEntities()) {
            bool is_selected = (startDijkstraVertexName == e->getName().c_str());
            if (ImGui::Selectable(e->getName().c_str(), is_selected)) {
                startDijkstraVertexName = e->getName().c_str();
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
#pragma endregion
#pragma region dijkstra-logic
    if (flag) {
        if (!dijkstraResult) {
            if (startDijkstraVertexName == nullptr) {
                this->createMessage("Please, select start vertex", 0.5f);
            } else {
                this->createMessage("Couldn't dijkstra", 0.5f);
            }
            flag = false;
        } else {
            for (const auto &pair: dijkstraResult.value()) {
                auto v = this->mouseEventDispatcher->getEntityByName(pair.first);
                auto *vertex = dynamic_cast<VertexEntity *>(v);
                if (vertex) {
                    auto &w = pair.second;
                    if (w != GraphHelpers::infitity) {
                        vertex->setAdditionalLabelString(std::to_string(w));
                    } else {
                        vertex->setAdditionalLabelString("inf");
                    }
                    vertex->setShowAdditionalLabel(true);
                }
            }
            flag = false;
            dijkstraResult = std::nullopt;
        }
    }
    if (!flag) {
        if (ImGui::Button("Remove upper vertex label")) {
            for (auto &entity: this->mouseEventDispatcher->getEntities()) {
                auto vertex = dynamic_cast<VertexEntity *>(entity.get());
                if (vertex) {
                    vertex->setShowAdditionalLabel(false);
                }
            }
        }
    }
#pragma endregion
#pragma region source-destination
    static const char *source = nullptr;
    static const char *destination = nullptr;
    ImGui::Text("source");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.2f);
    if (ImGui::BeginCombo("##", source)) {
        for (auto &e: this->mouseEventDispatcher->getEntities()) {
            bool is_selected = (source == e->getName().c_str());
            if (ImGui::Selectable(e->getName().c_str(), is_selected)) {
                source = e->getName().c_str();
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    ImGui::SameLine();
    ImGui::Text("destination");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.2f);
    if (ImGui::BeginCombo("###", destination)) {
        for (auto &e: this->mouseEventDispatcher->getEntities()) {
            bool is_selected = (destination == e->getName().c_str());
            if (ImGui::Selectable(e->getName().c_str(), is_selected)) {
                destination = e->getName().c_str();
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
#pragma endregion
#pragma region pathButton

    static std::optional<std::list<std::string>> path = std::nullopt;
    static bool pathFlag = false;
    ImGui::SameLine();
    if (ImGui::Button("path ")) {
        path = GraphHelpers::DijkstraPath(this->graph, source, destination);
        if (path) {
            pathFlag ^= true;
        }
    }
#pragma endregion
#pragma region pathButtonLogic
    if (pathFlag && path) {
        setPathColor(path.value());
        pathFlag = false;
        path = std::nullopt;
    }
#pragma endregion
#pragma region kruskal-button
    static bool kruskalFlag = false;
    static std::optional<std::list<std::pair<std::string, std::string>>> kruskalResult = std::nullopt;
    if (ImGui::Button("Kruskal MST")) {
        kruskalResult = GraphHelpers::KruskalMST(this->graph);
        if (kruskalResult) {
            kruskalFlag ^= true;
        } else {
            this->createMessage("Graph is oriented", 0.5f);

        }
    }
#pragma endregion
#pragma region kruskal-button-logic
    if (kruskalFlag) {
        if (kruskalResult) {
            const auto &list = kruskalResult.value();
            setPathColor(list);
            kruskalFlag = false;
            kruskalResult = std::nullopt;
        }
    }
#pragma endregion
#pragma region prima-buttom
    static bool primaFlag = false;
    static auto primaResult = std::optional<std::list<std::pair<std::string, std::string>>>(std::nullopt);
    if (ImGui::Button("PrimaMST")) {
        primaFlag ^= true;
        if (primaFlag) {
            if (startPrimaVertexName != nullptr) {
                primaResult = GraphHelpers::PrimaMST(this->graph, startPrimaVertexName);
            }
        }
    }
    ImGui::SameLine();
#pragma endregion
#pragma region VertexList
    ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.2f);
    if (ImGui::BeginCombo("Prima start vertex name", startPrimaVertexName)) {
        for (auto &e: this->mouseEventDispatcher->getEntities()) {
            bool is_selected = (startPrimaVertexName == e->getName().c_str());
            if (ImGui::Selectable(e->getName().c_str(), is_selected)) {
                startPrimaVertexName = e->getName().c_str();
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
#pragma endregion
#pragma region prima-button-logic
    if (primaFlag) {
        if (startPrimaVertexName == nullptr) {
            this->createMessage("Please, select start vertex", 0.5f);
        } else if (primaResult) {
            this->setPathColor(primaResult.value());
        } else {
            this->createMessage("Graph is oriented", 0.5f);
        }
        primaFlag = false;
        primaResult = std::nullopt;
    }
#pragma endregion
#pragma region shimbell
    static bool ShimbellButtonFlag = false;
    if (ImGui::Button("Shimbell")) {
        ShimbellButtonFlag = true;
    }


    if (ShimbellButtonFlag) {
        ImGui::Begin("Shimbell");
        static bool updateIncidentMatrixButtonFlag = true;
        static std::pair<std::vector<std::vector<int>>, std::vector<std::string>> incidentMatrix;
        static std::vector<std::vector<int>> shimbellMatrix;
        static int pow = 1;
        if (updateIncidentMatrixButtonFlag) {
            incidentMatrix = this->graph.getIncidentMatrix();
            shimbellMatrix = incidentMatrix.first;
            updateIncidentMatrixButtonFlag = false;
        }
        if (ImGui::Button("update incident matrix")) {
            updateIncidentMatrixButtonFlag = true;
        }
        this->imGuiDrawMatrix(incidentMatrix.second, incidentMatrix.first, "Incident Matrix");
        if (ImGui::InputInt("pow", &pow)) {
            if (pow >= 1) {
                shimbellMatrix = incidentMatrix.first;
                if (!shimbellMatrix.empty()) {
                    GraphHelpers::ShimbellPowMatrix(shimbellMatrix, pow);
                }
            } else {
                this->createMessage("Non negative pow", 0.5f);
            }
        }
        this->imGuiDrawMatrix(incidentMatrix.second, shimbellMatrix, "Incident Matrix");


        if (ImGui::Button("Close")) {
            ShimbellButtonFlag = false;
        }

        ImGui::End();
    }
#pragma endregion


#pragma region remove-path-button
    if (ImGui::Button("Remove paths")) {
        for (auto &e: this->arrowHolder->getEntities()) {
            auto *arrow = static_cast<Arrow *>(e.get());
            arrow->setLineColor(this->defaultColor);
        }
    }
#pragma endregion


    static bool bronKerboshFlag = false;
    static std::optional<std::list<std::vector<std::string>>> bronKerboshResult(std::nullopt);
    if (ImGui::Button("BronKerbosh")) {
        bronKerboshFlag ^= true;
        if (bronKerboshFlag) {
            bronKerboshResult = GraphHelpers::BronKerbrosh(this->graph);
        }
    }
    if (bronKerboshFlag) {


        if (bronKerboshResult) {
            bronKerboshFlag = false;
            auto start = 0.0f;
            auto step = 1.f / (float) bronKerboshResult.value().size();
            for (auto &vect: bronKerboshResult.value()) {
                auto color = ColorHelpers::interpolate(sf::Color::Red, sf::Color(60, 0, 100), start,
                                                       ColorHelpers::linear);
                this->setPathColor(vect, color, true);
                start += step;
            }

        }
//        bronKerboshResult = std::nullopt;
    }
    if (bronKerboshResult) {
        ImGui::NewLine();
        for (auto &v: bronKerboshResult.value()) {
            for (auto &s: v) {
                ImGui::SameLine();
                ImGui::Text("%s", s.c_str());
            }
            ImGui::NewLine();
        }

    }

#pragma region show-application-properties-button
    if (ImGui::Button("Show application properties")) {
        showApplicationProperties ^= true;
    }
#pragma endregion

#pragma region application-properties
    if (showApplicationProperties) {
        ImGui::Begin("Application properties");
#pragma region bgColor-editor
        static float color[3] = {static_cast<float>(this->bgColor.r) / 255.f,
                                 static_cast<float>(this->bgColor.g) / 255.f,
                                 static_cast<float>(this->bgColor.b) / 255.f};
        if (ImGui::ColorEdit3("Background color", color)) {
            bgColor.r = static_cast<sf::Uint8>(color[0] * 255.f);
            bgColor.g = static_cast<sf::Uint8>(color[1] * 255.f);
            bgColor.b = static_cast<sf::Uint8>(color[2] * 255.f);
        }
#pragma endregion
#pragma region path-color
        static float pathColor[3] = {static_cast<float>(this->pathColor.r) / 255.f,
                                     static_cast<float>(this->pathColor.g) / 255.f,
                                     static_cast<float>(this->pathColor.b) / 255.f};
        if (ImGui::ColorEdit3("Path color", pathColor)) {
            this->pathColor.r = static_cast<sf::Uint8>(pathColor[0] * 255.f);
            this->pathColor.g = static_cast<sf::Uint8>(pathColor[1] * 255.f);
            this->pathColor.b = static_cast<sf::Uint8>(pathColor[2] * 255.f);
        }
#pragma endregion
        ImGui::SliderFloat("Navigation speed", &this->viewMoveSpeed, 0.5f, 2000.f);
#pragma region close-button
        if (ImGui::Button("Close")) {
            showApplicationProperties = false;
        }
#pragma endregion
        ImGui::End();
    }
#pragma endregion

    ImGui::End();
}

void Application::imGuiDrawMatrix(const std::vector<std::string> &header, const std::vector<std::vector<int>> &matrix,
                                  const char *matrixId) const {
    if (ImGui::BeginTable(matrixId, std::max(header.size() + 1, (std::size_t) 1))) {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text(" ");
        static ImU32 bgColor = ImGui::GetColorU32(ImVec4(0.3f, 0.3f, 0.7f, 1.0f));
        for (int column = 0; column < header.size(); ++column) {
            ImGui::TableSetColumnIndex(column + 1);
            ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, bgColor);
            ImGui::Text("%s", header[column].c_str());
        }
        for (int i = 0; i < header.size(); ++i) {
            ImGui::TableNextRow();
            for (int j = 0; j < header.size() + 1; ++j) {
                ImGui::TableSetColumnIndex(j);
                if (j == 0) {
                    ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, bgColor);
                    ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.1f);
                    ImGui::Text("%s", header[i].c_str());
                } else {
                    ImGui::Text("%d", matrix[i][j - 1]);
                }
            }
        }
        ImGui::EndTable();
    }
}

void Application::setPathColor(const std::list<std::pair<std::string, std::string>> &list) {
    for (const auto &edge: list) {
        auto &firstName = edge.first;
        auto &secondName = edge.second;
        auto foundFirst = std::find_if(arrowHolder->getEntities().begin(),
                                       arrowHolder->getEntities().end(),
                                       [&firstName, &secondName](const std::shared_ptr<Entity> &e) -> bool {
                                           auto *arrow = dynamic_cast<Arrow *>(e.get());
                                           if (!arrow) {
                                               return false;
                                           }
                                           return arrow->getSourceName() == firstName &&
                                                  arrow->getDestinationName() == secondName;
                                       });
        auto foundSecond = std::find_if(arrowHolder->getEntities().begin(),
                                        arrowHolder->getEntities().end(),
                                        [&firstName, &secondName](const std::shared_ptr<Entity> &e) -> bool {
                                            auto *arrow = dynamic_cast<Arrow *>(e.get());
                                            if (!arrow) {
                                                return false;
                                            }
                                            return arrow->getSourceName() == secondName &&
                                                   arrow->getDestinationName() == firstName;
                                        });
        if (foundFirst != arrowHolder->getEntities().end()) {
            auto *arrow = dynamic_cast<Arrow *>(foundFirst->get());
            arrow->setLineColor(pathColor);
        }
        if (foundSecond != arrowHolder->getEntities().end()) {
            auto *arrow = dynamic_cast<Arrow *>(foundSecond->get());
            arrow->setLineColor(pathColor);
        }
    }
}

void Application::setPathColor(std::list<std::string> &path) {
    auto iter = path.begin();
    auto end = std::next(iter, (std::ptrdiff_t) (path.size() - 1));
    for (; iter != end; ++iter) {
        auto &current = iter;
        auto next = std::next(iter);
        auto &firstName = *current;
        auto &secondName = *next;
        auto foundFirst = std::find_if(arrowHolder->getEntities().begin(),
                                       arrowHolder->getEntities().end(),
                                       [&firstName, &secondName](const std::shared_ptr<Entity> &e) -> bool {
                                           auto *arrow = dynamic_cast<Arrow *>(e.get());
                                           if (!arrow) {
                                               return false;
                                           }
                                           return arrow->getSourceName() == firstName &&
                                                  arrow->getDestinationName() == secondName;
                                       });
        auto foundSecond = std::find_if(arrowHolder->getEntities().begin(),
                                        arrowHolder->getEntities().end(),
                                        [&firstName, &secondName](const std::shared_ptr<Entity> &e) -> bool {
                                            auto *arrow = dynamic_cast<Arrow *>(e.get());
                                            if (!arrow) {
                                                return false;
                                            }
                                            return arrow->getSourceName() == secondName &&
                                                   arrow->getDestinationName() == firstName;
                                        });
        if (foundFirst != arrowHolder->getEntities().end()) {
            auto *arrow = static_cast<Arrow *>(foundFirst->get());
            arrow->setLineColor(pathColor);
        }
        if (foundSecond != arrowHolder->getEntities().end()) {
            auto *arrow = static_cast<Arrow *>(foundSecond->get());
            arrow->setLineColor(pathColor);
        }

    }
}

void Application::createMessage(const std::string &text, float ttl) {
    auto message = std::make_shared<Message>(this->font, text, ttl, this->gui_view);
    message->setInViewPositionFactors(0.5, 0.9f);
    this->guiEventDispatcher->addEntity(message);
}

Application::~Application() {
    ImGui::SFML::Shutdown();
}

void Application::onSortBtnClicked(void *) {
    auto sort = GraphHelpers::TopologicalSort(this->graph);
    if (!sort) {
        this->createMessage("Couldn't sort", 0.5f);
        return;
    }
    auto &map = *sort;
    auto windowSize = this->window.getSize();
    auto &width = windowSize.x;
    auto &height = windowSize.y;
    auto stepHeight = height / (map.size() + 1);
    unsigned long long stepWidth{};
    std::size_t currStepY = 1;
    for (auto &order: map) {
        stepWidth = width / (order.second.size() + 1);
        std::size_t currStepX = 1;
        for (auto &vertexName: order.second) {
            auto x = stepWidth * currStepX;
            auto y = stepHeight * currStepY;

            sf::Vector2f posInView =
                    this->window.mapPixelToCoords(
                            sf::Vector2i(
                                    static_cast<int>(x),
                                    static_cast<int>(y)),
                            this->view);
            auto entity = this->mouseEventDispatcher->getEntityByName(vertexName);
            entity->setPosition(posInView);
            ++currStepX;
        }
        ++currStepY;
    }
}

void Application::onLabelTextEntered(void *param) {
    auto text = label->getString();
    createVertexByName(text);
}

void Application::createVertexByName(const std::string &text) {
    if (graph.contains(text)) {
        createMessage("Vertex already exists...", 0.5f);
        return;
    }
    if (text.empty()) {
        createMessage("Vertex name should be not empty...", 0.5f);
        return;
    }
    label->setString("");
    auto e = std::make_unique<VertexEntity>(font, text, mppWorldPos);
    e->getSignal(onMiddleMouseClicked).addSlot(
            ByPressingRightMouseButtonOnVertex(this, e.get()));
    e->getSignal(onRightMouseClicked).addSlot([this, entity{e.get()}](void *) {
        entityBounder->pushEntity(entity);
    });
    auto pos = mppWorldPos->getMousePosition();
    e->followMouse(0, 0);
    e->setPosition(pos.first, pos.second);
    mouseEventDispatcher->addEntity(std::move(e));
    graph.addVertex(text);
    createMessage("Vertex created", 0.5f);
    colorizeVertices();
}

void Application::setPathColor(std::vector<std::string> &path, sf::Color pathColor, bool cycle) {
    auto iter = path.begin();
    auto start = path.begin();
    std::vector<std::string>::iterator end;
    end = std::next(iter, (std::ptrdiff_t) (path.size() - 1));
    if (cycle) {
        for (int i = 0; i < path.size() - 1; ++i) {
            for (int j = i + 1; j < path.size(); ++j) {
                auto &firstName = path[i];
                auto &secondName = path[j];
                setEdgeColor(pathColor, firstName, secondName);
            }
        }
        return;
    }
    for (; iter != end; ++iter) {
        auto &current = iter;
        auto next = std::next(iter);
        auto &firstName = *current;
        auto &secondName = *next;
        setEdgeColor(pathColor, firstName, secondName);
    }

}

void Application::setEdgeColor(const sf::Color &pathColor, const std::string &firstName,
                               const std::string &secondName) const {
    auto foundFirst = std::find_if(arrowHolder->getEntities().begin(),
                                   arrowHolder->getEntities().end(),
                                   [&firstName, &secondName](const std::shared_ptr<Entity> &e) -> bool {
                                       auto *arrow = dynamic_cast<Arrow *>(e.get());
                                       if (!arrow) {
                                           return false;
                                       }
                                       return arrow->getSourceName() == firstName &&
                                              arrow->getDestinationName() == secondName;
                                   });
    auto foundSecond = std::find_if(arrowHolder->getEntities().begin(),
                                    arrowHolder->getEntities().end(),
                                    [&firstName, &secondName](const std::shared_ptr<Entity> &e) -> bool {
                                        auto *arrow = dynamic_cast<Arrow *>(e.get());
                                        if (!arrow) {
                                            return false;
                                        }
                                        return arrow->getSourceName() == secondName &&
                                               arrow->getDestinationName() == firstName;
                                    });
    if (foundFirst != arrowHolder->getEntities().end()) {
        auto *arrow = dynamic_cast<Arrow *>(foundFirst->get());
        arrow->setLineColor(pathColor);
    }
    if (foundSecond != arrowHolder->getEntities().end()) {
        auto *arrow = dynamic_cast<Arrow *>(foundSecond->get());
        arrow->setLineColor(pathColor);
    }
}

Application::ByPressingRightMouseButtonOnVertex::ByPressingRightMouseButtonOnVertex(Application *app, Entity *e) {
    this->app = app;
    this->e = e;
}

void Application::ByPressingRightMouseButtonOnVertex::operator()(void *param) {
    if (app->entityBounder->isBounding()) {
        app->createMessage("Cannot remove vertex while creating edge", 0.5f);
        return;
    }
    e->markToRemove(true);
    app->graph.removeVertex((e->getName()));
    app->createMessage("Vertex removed", 0.5f);
    app->colorizeVertices();
}
