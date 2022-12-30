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