//
// Created by ASUS on 31.12.2022.
//

#include <Entity.h>

const std::string &Entity::getName() const{
    return this->name;
}

Signal &Entity::getSignal(const size_t &id) {
    return this->signals.get(id);
}

void Entity::setName(const std::string &name) {
    this->name = name;
}
sf::FloatRect Entity::getGlobalBounds() const {
    return {0.f,0.f,0.f,0.f};
}

void Entity::setPosition(float x, float y) {
    Transformable::setPosition(x, y);
}

void Entity::setPosition(const sf::Vector2f & position) {
    Transformable::setPosition(position);
}

void Entity::move(float x, float y) {
    auto pos = this->getPosition();
    this->setPosition(pos + sf::Vector2f(x,y));
}

void Entity::move(const sf::Vector2f &vect) {
    this->move(vect.x,vect.y);
}

