//
// Created by ASUS on 30.12.2022.
//

#include <iostream>
#include "Application.h"

Application::Application(int width, int height, const std::string &title) :
        window(sf::VideoMode(width, height), title),
        bgColor(sf::Color::White) {
    this->window.setVerticalSyncEnabled(true);
    this->eventDispatcher.addListenerOnEvent(sf::Event::MouseMoved,[](const sf::Event& event,const sf::Time& dt)
    {
       std::cout << event.mouseMove.x << ' '<< event.mouseMove.y << std::endl;
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



void Application::processEvents(sf::Event &event, const sf::Time &dt) {
    while (this->window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            this->window.close();
        }
        this->eventDispatcher.handleEvent(event,dt);
    };
    this->eventDispatcher.handleInput(dt);
}

void Application::update(const sf::Time &dt) {

}

void Application::render(const sf::Time &dt) {
    this->window.clear(this->bgColor);

    this->window.display();
}
