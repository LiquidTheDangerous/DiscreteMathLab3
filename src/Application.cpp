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
#include <complex>


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
//    updateArrows(dt);

    this->guiEventDispatcher->update(dt.asSeconds());
}

void Application::updateArrows(const sf::Time &dt) {
    bool rm = false;

    for (auto &item: arrows) {
        item->update(dt.asSeconds());
        rm |= item->needsRemoved();
    }
    if (rm) {
        arrows.remove_if([](auto &item) -> bool { return item->needsRemoved(); });
    }
}

void Application::render(const sf::Time &dt) {
    this->window.clear(this->bgColor);
    this->window.setView(this->view);
    this->window.draw(*this->mouseEventDispatcher);

    this->window.draw(*this->arrowHolder);
//    drawArrows();

    this->window.setView(this->gui_view);
    this->window.draw(*this->guiEventDispatcher);

    imguiWindow();

    ImGui::SFML::Render(window);
    this->window.display();
}

void Application::imguiWindow() {
    ImGui::Begin("Functions");
    ImGui::StyleColorsDark();

#pragma region createVertex
    static char buf[255];
    static const char *startVertexName = nullptr;
    ImGui::InputText("- vertex name", buf, 255);
    if (ImGui::Button("Create vertex")) {
        this->createVertexByName(buf);
    }
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
            if (startVertexName != nullptr) {
                dijkstraResult = GraphHelpers::Dijkstra(this->graph, startVertexName);
            }
        }
    }
    ImGui::SameLine();
#pragma endregion
#pragma region VertexList
    ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.2f);
    if (ImGui::BeginCombo("Start Vertex Name", startVertexName)) {
        for (auto &e: this->mouseEventDispatcher->getEntities()) {
            bool is_selected = (startVertexName == e->getName().c_str());
            if (ImGui::Selectable(e->getName().c_str(), is_selected)) {
                startVertexName = e->getName().c_str();
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
            if (startVertexName == nullptr) {
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
#pragma region remove-path-button
    if (ImGui::Button("Remove paths")) {
        for (auto &e: this->arrowHolder->getEntities()) {
            auto *arrow = static_cast<Arrow *>(e.get());
            arrow->setLineColor(this->defaultColor);
        }
    }
#pragma endregion

    ImGui::End();
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

void Application::drawArrows() {
    for (auto &item: arrows) {
        window.draw(*item);
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
