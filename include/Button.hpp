//
// Created by ASUS on 05.02.2023.
//

#ifndef DISCRETEMATHLAB3_BUTTON_HPP
#define DISCRETEMATHLAB3_BUTTON_HPP

#include <Entity.h>
#include <signals.hpp>
#include "GuiComponent.hpp"

class Button : public GUIComponent {

    sf::RectangleShape rectangle;
    bool rm_flag = false;
    sf::Text text;
    sf::Color defaultBgColor;
    sf::Color onHoverBgColor;
    sf::Color onClickedBgColor;
public:

    Button(float w, float h, sf::Font &font, const std::string &text, sf::View &view, const sf::Color &textColor = sf::Color::Black,
           const sf::Color &bgColor = sf::Color::White, const sf::Color &outlineColor = sf::Color::Black);

    sf::FloatRect getGlobalBounds() const override;

    void update(float dt) override;

    void markToRemove(bool remove) override;

    bool needsRemoved() const override;

    void setOnHoverBgColor(const sf::Color &color);

    void setDefaultBgColor(const sf::Color &color);

    void setOnClickedBgColor(const sf::Color& color);

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

};


#endif //DISCRETEMATHLAB3_BUTTON_HPP
