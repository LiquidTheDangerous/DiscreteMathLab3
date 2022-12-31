//
// Created by ASUS on 30.12.2022.
//

#include <iostream>
#include <Application.h>
#include "MousePositionProviderImpl.h"

Application::Application(int width, int height, const std::string &title) :
        window(sf::VideoMode(width, height), title),
        bgColor(sf::Color::White) {
    this->window.setVerticalSyncEnabled(true);
    if (!this->font.loadFromFile("../res/Montserrat_Regular.ttf")) {
        std::cerr << "ERROR LOADING FONT" << std::endl;
    };
    auto& AppWindow = this->window;
    this->eventDispatcher.addListenerOnEvent(sf::Event::Resized,[&AppWindow](const sf::Event& event, const sf::Time& dt)
    {
        auto view = sf::View(sf::Vector2f((float)event.size.width/2,(float)event.size.height/2),sf::Vector2f((float)event.size.width,(float)event.size.height));
        AppWindow.setView(view);
    });

    auto e = std::make_shared<VertexEntity>(this->font, "123");
    e->move(100, 100);
    auto mpp = std::make_shared<MousePositionProviderImpl>(&this->window);
    e->setMousePositionProvider(mpp);
    this->mouseEventDispatcher.setMousePositionProvider(mpp);
    this->mouseEventDispatcher.addEntity(e);
    e->getSignal(signals::onLeftMouseClicked).addSlot([e]() {

        e->followMouse(0,0);
    });
    e->getSignal(signals::onLeftMouseReleased).addSlot([e]() {
        e->unfollowMouse();
    });
    e->getSignal(signals::onMouseEntered).addSlot([e]() { e->setCircleFillColor(sf::Color::Black); });
    e->getSignal(signals::onMouseLeaved).addSlot([e]() { e->setCircleFillColor(sf::Color::Red); });

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
        this->mouseEventDispatcher.handleEvent(event);
    };
    this->eventDispatcher.handleInput(dt);
}

void Application::update(const sf::Time &dt) {

    this->mouseEventDispatcher.update(dt.asSeconds());

}

void Application::render(const sf::Time &dt) {
    this->window.clear(this->bgColor);
    this->window.draw(this->mouseEventDispatcher);
    this->window.display();
}
