//
// Created by ASUS on 31.12.2022.
//

#ifndef DISCRETEMATHLAB3_MOUSEPOSITIONPROVIDERIMPL_H
#define DISCRETEMATHLAB3_MOUSEPOSITIONPROVIDERIMPL_H


#include <MousePositionProvider.h>
#include <SFML/Graphics.hpp>

class MousePositionProviderImpl : public MousePositionProvider {
    sf::RenderWindow *window;
public:
    MousePositionProviderImpl(sf::RenderWindow *window) {
        this->window = window;
    }

    [[nodiscard]] std::pair<float, float> getMousePosition() const override {
        auto pos = window->mapPixelToCoords(sf::Mouse::getPosition(*this->window));
        return {pos.x,pos.y};
    }

};


#endif //DISCRETEMATHLAB3_MOUSEPOSITIONPROVIDERIMPL_H
