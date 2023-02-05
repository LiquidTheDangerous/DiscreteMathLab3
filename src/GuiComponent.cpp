//
// Created by ASUS on 05.02.2023.
//

#include "Button.hpp"
#include <iostream>
#include "GuiComponent.hpp"

void GUIComponent::setInViewPositionFactors(float w, float h) {
    this->viewFactorW = w;
    this->viewFactorH = h;
}

std::pair<float, float> GUIComponent::getViewPositionFactors() {
    return {this->viewFactorW,this->viewFactorH};
}

GUIComponent::GUIComponent(sf::View &view,float factx, float facty) :
    view(&view),
    viewFactorW(factx),
    viewFactorH(facty),
    positionOffsetX(0),
    positionOffsetY(0){
}

void GUIComponent::setPosition(float x, float y) {
    this->positionOffsetX = x;
    this->positionOffsetY = y;
}

void GUIComponent::setPosition(const sf::Vector2f &f) {
    this->positionOffsetX = f.x;
    this->positionOffsetY = f.y;
}

void GUIComponent::update(float dt) {
    auto viewSize = this->view->getSize();
    this->Transformable::setPosition(viewSize.x * this->viewFactorW + this->positionOffsetX, viewSize.y * this->viewFactorH + this->positionOffsetY);
}
