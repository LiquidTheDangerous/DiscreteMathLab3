//
// Created by ASUS on 05.02.2023.
//

#include <iostream>
#include "Button.hpp"

sf::FloatRect Button::getGlobalBounds() const {
    return this->getTransform().transformRect(this->rectangle.getGlobalBounds());
}

void Button::update(float dt) {
    GUIComponent::update(dt);
}

void Button::markToRemove(bool remove) {
    if (remove) {
        this->signals.emit(signals::onDelete, nullptr);
    }
    this->rm_flag = remove;
}

bool Button::needsRemoved() const {
    return this->rm_flag;
}

void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= this->getTransform();
    target.draw(this->rectangle, states);
    if (this->text.getString().getSize() >= 0){
        target.draw(this->text,states);
    }
}

Button::Button(float w, float h, sf::Font &font, const std::string &text, sf::View &view, const sf::Color &textColor,
               const sf::Color &bgColor, const sf::Color &outlineColor) :
        rectangle(sf::Vector2f(w, h)),
        text(text, font),
        defaultBgColor(bgColor),
        onHoverBgColor(bgColor),
        onClickedBgColor(bgColor),
        GUIComponent(view){
    this->rectangle.setFillColor(bgColor);
    this->rectangle.setOutlineThickness(1.f);
    this->rectangle.setOutlineColor(outlineColor);
    this->text.setFillColor(textColor);
    this->getSignal(signals::onLeftMouseClicked).addSlot([this](void *)
    {
        this->rectangle.setFillColor(this->onClickedBgColor);
    });
    this->getSignal(signals::onLeftMouseReleased).addSlot([this](void*){
        this->rectangle.setFillColor(this->defaultBgColor);
    });
    this->getSignal(signals::onMouseEntered).addSlot([this](void*){
       this->rectangle.setFillColor(this->onHoverBgColor);
    });
    this->getSignal(signals::onMouseLeaved).addSlot([this](void*){
       this->rectangle.setFillColor(this->defaultBgColor);
    });

}

void Button::setDefaultBgColor(const sf::Color &color) {
    this->defaultBgColor = color;
}

void Button::setOnHoverBgColor(const sf::Color &color) {
    this->onHoverBgColor = color;
}

void Button::setOnClickedBgColor(const sf::Color &color) {
    this->onClickedBgColor = color;
}



