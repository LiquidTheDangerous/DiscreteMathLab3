//
// Created by ASUS on 31.12.2022.
//

#include <EntityEventDispatcherImpl.h>
#include <iostream>
#include <utility>

void EntityEventDispatcherImpl::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= this->getTransform();
    for (auto &entity: this->entities) {
        target.draw(*entity, states);
    }
}

void EntityEventDispatcherImpl::addEntity(const std::shared_ptr<Entity> &entity) {
    this->entities.push_back(entity);
}

void EntityEventDispatcherImpl::handleEvent(const sf::Event &event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        auto pos = this->mousePositionProvider->getMousePosition();
        for (auto &e: this->entities) {
            auto bounds = e->getGlobalBounds();
            if (bounds.contains(pos.first, pos.second)) {
                switch (event.mouseButton.button) {
                    case sf::Mouse::Left:
                        e->signals.emit(signals::onLeftMouseClicked, (void *) &event);
                        break;
                    case sf::Mouse::Right:
                        e->signals.emit(signals::onRightMouseClicked, (void *) &event);
                        break;
                    case sf::Mouse::Middle:
                        e->signals.emit(signals::onMiddleMouseClicked, (void *) &event);
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
                        e->signals.emit(signals::onLeftMouseReleased, (void *) &event);
                        break;
                    case sf::Mouse::Right:
                        e->signals.emit(signals::onRightMouseReleased, (void *) &event);
                        break;
                    case sf::Mouse::Middle:
                        e->signals.emit(signals::onMiddleMouseReleased, (void *) &event);
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
                    e->signals.emit(signals::onMouseEntered, (void *) &event);
                    e_id = c;
                }
            } else {
                if (e_id == c) {
                    e->signals.emit(signals::onMouseLeaved, (void *) &event);
                    e_id = -1;
                }
            }
            ++c;
        }
    }
    if (event.type == sf::Event::TextEntered) {
        for (auto &e: this->entities) {
            e->signals.emit(signals::onEnteredText, (void *) &event);
        }
    }
}

void EntityEventDispatcherImpl::setMousePositionProvider(std::shared_ptr<MousePositionProvider> MousePositionProvider) {
    this->mousePositionProvider = std::move(MousePositionProvider);
}

void EntityEventDispatcherImpl::update(float dt) {
    auto nr = false;
    for (auto &e: this->entities) {
        e->update(dt);
        nr |= e->needsRemoved();
    }
    if (nr){
        this->removeEntities();
    }
}

void EntityEventDispatcherImpl::markToRemove(bool remove) {

}

bool EntityEventDispatcherImpl::needsRemoved() const {
    return false;
}

void EntityEventDispatcherImpl::removeEntities() {
    auto iter = std::remove_if(this->entities.begin(), this->entities.end(), [](const std::shared_ptr<Entity>& e)->bool
    {
        return e->needsRemoved();
    });
    this->entities.erase(iter,this->entities.end());
}

std::size_t EntityEventDispatcherImpl::getEntitiesSize() const {
    return this->entities.size();
}

Entity *EntityEventDispatcherImpl::getEntityByName(const std::string &name) const {
    auto iter = std::find_if(this->entities.begin(), this->entities.end(), [&name](const auto& entity)->bool
    {
        return entity->getName() == name;
    });
    if (iter == this->entities.end()){
        return nullptr;
    }

    return iter->get();
}
