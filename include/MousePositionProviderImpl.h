//
// Created by ASUS on 31.12.2022.
//

#ifndef DISCRETEMATHLAB3_MOUSEPOSITIONPROVIDERIMPL_H
#define DISCRETEMATHLAB3_MOUSEPOSITIONPROVIDERIMPL_H


#include <MousePositionProvider.h>
#include <SFML/Graphics.hpp>

class MousePositionProviderImpl : public MousePositionProvider {
    sf::RenderWindow *window;
    sf::View* view;
public:
    MousePositionProviderImpl(sf::RenderWindow *window, sf::View* view) {
        this->view = view;
        this->window = window;
    }

    [[nodiscard]] std::pair<float, float> getMousePosition() const override {
        auto pos = window->mapPixelToCoords(sf::Mouse::getPosition(*this->window),*this->view);
        return {pos.x,pos.y};
    }

};


#endif //DISCRETEMATHLAB3_MOUSEPOSITIONPROVIDERIMPL_H
