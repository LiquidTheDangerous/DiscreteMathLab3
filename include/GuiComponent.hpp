//
// Created by ASUS on 05.02.2023.
//

#ifndef DISCRETEMATHLAB3_GUICOMPONENT_HPP
#define DISCRETEMATHLAB3_GUICOMPONENT_HPP


#include "signals.hpp"
#include "Entity.h"

class GUIComponent : public Entity {

protected:
    sf::View *view;
    float viewFactorH;
    float viewFactorW;
    float positionOffsetX;
    float positionOffsetY;
public:
    explicit GUIComponent(sf::View &view, float viewFactorW = 0, float viewFactorH = 0);

    std::pair<float, float> getViewPositionFactors();

    void update(float dt) override;

    void setInViewPositionFactors(float w, float h);

    void setPosition(float x, float y) override;

    void setPosition(const sf::Vector2f &f) override;


};


#endif //DISCRETEMATHLAB3_GUICOMPONENT_HPP
