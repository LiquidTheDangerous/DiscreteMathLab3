//
// Created by ASUS on 31.12.2022.
//

#include <EntityContainer.h>

void EntityContainer::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= this->getTransform();
    for (auto &entity: this->entities) {
        target.draw(*entity, states);
    }
}

void EntityContainer::addEntity(const std::shared_ptr<Entity> &entity) {
    this->entities.push_back(entity);
}

void EntityContainer::handleEvent(const sf::Event &event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            auto x = event.mouseButton.x;
            auto y = event.mouseButton.y;
            for (auto &e: this->entities) {
                auto bounds = e->getGlobalBounds();
                if (bounds.contains((float)x,(float) y)) {
                    e->signals.emit(signals::onLeftMouseClicked);
                }
            }
        }
    }
}
