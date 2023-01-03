//
// Created by ASUS on 30.12.2022.
//

#include <iostream>
#include <Application.h>
#include <MousePositionProviderImpl.h>
#include <Label.h>
#include <Message.h>
#include <Arrow.h>

Application::Application(int width, int height, const std::string &title) :
        window(sf::VideoMode(width, height), title),
        bgColor(sf::Color::White) {
    this->entityBounder = std::make_shared<EntityBounder>(this);
    this->mouseEventDispatcher = std::make_shared<EntityEventDispatcherImpl>();
    this->guiEventDispatcher = std::make_shared<EntityEventDispatcherImpl>();
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


    auto label = std::make_shared<Label>(this->font, "Enter Vertex Name");
    label->move(600, 0);
    label->getSignal(signals::onEndEditingText).addSlot([label{label.get()}, this](void *param) {
        auto text = label->getString();
        if (this->graph.contains(text)) {
            this->createMessage("Vertex already exists...", 0.5f);
            return;
        }
        if (text.empty()) {
            this->createMessage("Vertex name should be not empty...", 0.5f);
            return;
        }
        label->setString("");
        auto e = std::make_unique<VertexEntity>(this->font, text, this->mppWorldPos);
        e->getSignal(signals::onMiddleMouseClicked).addSlot(
                Application::ByPressingRightMouseButtonOnVertex(this, e.get()));
        e->getSignal(signals::onRightMouseClicked).addSlot([this, entity{e.get()}](void *) {
            this->entityBounder->pushEntity(entity);
        });
        auto pos = mppWorldPos->getMousePosition();
        e->followMouse(0, 0);
        e->setPosition(pos.first, pos.second);
        this->mouseEventDispatcher->addEntity(std::move(e));
        this->graph.addVertex(text);
        this->createMessage("Vertex created", 0.5f);
    });


    this->mouseEventDispatcher->setMousePositionProvider(mppWorldPos);

    this->guiEventDispatcher->setMousePositionProvider(mppGUI);

    this->guiEventDispatcher->addEntity(label);
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


void Application::processEvents(sf::Event &event, const sf::Time &dt) {
    while (this->window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            this->window.close();
        }
        this->eventDispatcher.handleEvent(event, dt);
        this->mouseEventDispatcher->handleEvent(event);
        this->guiEventDispatcher->handleEvent(event);
    };
    this->eventDispatcher.handleInput(dt);
}

void Application::update(const sf::Time &dt) {

    this->mouseEventDispatcher->update(dt.asSeconds());
    updateArrows(dt);

    this->guiEventDispatcher->update(dt.asSeconds());
}

void Application::updateArrows(const sf::Time &dt) {
    bool rm = false;

    for (auto &item: arrows) {
        item->update(dt.asSeconds());
         rm |= item->needsRemoved();
    }
    if (rm){
        arrows.remove_if([](auto& item)->bool{return item->needsRemoved();});
    }
}

void Application::render(const sf::Time &dt) {
    this->window.clear(this->bgColor);
    this->window.setView(this->view);
    this->window.draw(*this->mouseEventDispatcher);

    for (auto &item: this->arrows) {
        this->window.draw(*item);
    }

    this->window.setView(this->gui_view);
    this->window.draw(*this->guiEventDispatcher);

    this->window.display();
}

void Application::createMessage(const std::string &text, float ttl) {
    auto message = std::make_shared<Message>(this->font, text, ttl);
    auto w_size = this->window.getSize();
    message->setPosition((float) w_size.x / 2, (float) w_size.y - 100);
    this->guiEventDispatcher->addEntity(message);
}
