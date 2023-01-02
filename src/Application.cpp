//
// Created by ASUS on 30.12.2022.
//

#include <iostream>
#include <Application.h>
#include "MousePositionProviderImpl.h"
#include "Label.h"

Application::Application(int width, int height, const std::string &title) :
        window(sf::VideoMode(width, height), title),
        bgColor(sf::Color::White) {
    this->mouseEventDispatcher = std::make_shared<EntityEventDispatcherImpl>();
    this->window.setVerticalSyncEnabled(true);
    if (!this->font.loadFromFile("../res/Montserrat_Regular.ttf")) {
        std::cerr << "ERROR LOADING FONT" << std::endl;
    };
    this->eventDispatcher.addListenerOnEvent(sf::Event::Resized,
                                             [this](const sf::Event &event, const sf::Time &dt) {
                                                 auto view = sf::View(sf::Vector2f((float) event.size.width / 2,
                                                                                   (float) event.size.height / 2),
                                                                      sf::Vector2f((float) event.size.width,
                                                                                   (float) event.size.height));
                                                 this->window.setView(view);
                                             });


    auto mpp = std::make_shared<MousePositionProviderImpl>(&this->window);


    auto e = std::make_shared<VertexEntity>(this->font, "123", mpp);
    e->move(100, 100);
    e->setMousePositionProvider(mpp);

    auto label = std::make_shared<Label>(this->font,"Enter Vertex Name");
    label->move(300,300);
    this->mouseEventDispatcher->setMousePositionProvider(mpp);
    this->mouseEventDispatcher->addEntity(e);
    this->mouseEventDispatcher->addEntity(label);
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
    };
    this->eventDispatcher.handleInput(dt);
}

void Application::update(const sf::Time &dt) {

    this->mouseEventDispatcher->update(dt.asSeconds());

}

void Application::render(const sf::Time &dt) {
    this->window.clear(this->bgColor);
    this->window.draw(*this->mouseEventDispatcher);


    this->window.display();
}
