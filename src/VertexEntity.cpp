//
// Created by ASUS on 30.12.2022.
//

#include <VertexEntity.h>

void VertexEntity::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= this->getTransform();
    target.draw(this->circle, states);
    target.draw(this->text, states);
}

VertexEntity::VertexEntity(sf::Font &font, const std::string &name) {
    this->text.setFont(font);
    this->setName(name);
    this->circle.setRadius(20.f);
    this->circle.move(-20.f, -20.f);
    this->circle.setFillColor(sf::Color::Red);
}

sf::FloatRect VertexEntity::getGlobalBounds() const {
    return this->getTransform().transformRect(this->circle.getGlobalBounds());

}


