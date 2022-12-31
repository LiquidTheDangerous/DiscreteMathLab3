//
// Created by ASUS on 31.12.2022.
//

#include <MouseEventDispatcher.h>
#include <iostream>
#include <utility>

void MouseEventDispatcher::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= this->getTransform();
    for (auto &entity: this->entities) {
        target.draw(*entity, states);
    }
}

void MouseEventDispatcher::addEntity(const std::shared_ptr<Entity> &entity) {
    this->entities.push_back(entity);
}

void MouseEventDispatcher::handleEvent(const sf::Event &event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        auto pos = this->mousePositionProvider->getMousePosition();
        for (auto &e: this->entities) {
            auto bounds = e->getGlobalBounds();
            if (bounds.contains(pos.first, pos.second)) {
                switch (event.mouseButton.button) {
                    case sf::Mouse::Left:
                        e->signals.emit(signals::onLeftMouseClicked);
                        break;
                    case sf::Mouse::Right:
                        e->signals.emit(signals::onRightMouseClicked);
                        break;
                    case sf::Mouse::Middle:
                        e->signals.emit(signals::onMiddleMouseClicked);
                        break;
                }
            }
        }
    }
    if (event.type == sf::Event::MouseButtonReleased) {
        auto pos = this->mousePositionProvider->getMousePosition();
        for (auto &e: this->entities) {
            auto bounds = e->getGlobalBounds();
            if (bounds.contains(pos.first, pos.second)) {
                switch (event.mouseButton.button) {
                    case sf::Mouse::Left:
                        e->signals.emit(signals::onLeftMouseReleased);
                        break;
                    case sf::Mouse::Right:
                        e->signals.emit(signals::onRightMouseReleased);
                        break;
                    case sf::Mouse::Middle:
                        e->signals.emit(signals::onMiddleMouseReleased);
                        break;
                }
            }
        }
    }
    if (event.type == sf::Event::MouseMoved) {
        auto pos = this->mousePositionProvider->getMousePosition();
        int c = 0;
        for (auto &e: this->entities) {

            auto bounds = e->getGlobalBounds();
            if (bounds.contains(pos.first, pos.second)) {
                if (e_id != c) {
                    e->signals.emit(signals::onMouseEntered);
                    e_id = c;
                }
            } else {
                if (e_id == c) {
                    e->signals.emit(signals::onMouseLeaved);
                    e_id = -1;
                }
            }
            ++c;
        }
    }
}

void MouseEventDispatcher::setMousePositionProvider(std::shared_ptr<MousePositionProvider> MousePositionProvider) {
    this->mousePositionProvider = std::move(MousePositionProvider);
}
