//
// Created by ASUS on 31.12.2022.
//

#ifndef DISCRETEMATHLAB3_LABEL_H
#define DISCRETEMATHLAB3_LABEL_H

#include <SFML/Graphics.hpp>
#include "Entity.h"
#include <GuiComponent.hpp>
class Label : public GUIComponent {
    std::string string_text;
    std::string placeholder;
    sf::Text text;
    sf::VertexArray line;
    bool selected = false;
    bool rm_flag;

    void reinitLine();

public:


    explicit Label(sf::Font &font, const std::string &placeholder, sf::View &view, const std::string &text = "");

    void setString(const std::string& text);

    void setPlaceholder(const std::string& placeholder);

    const std::string& getPlaceholder() const;

    bool isSelected() const;

    void setSelected(bool selected);

    const std::string& getString() const;

    sf::FloatRect getGlobalBounds() const override;

    void update(float dt) override;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    void markToRemove(bool remove) override;

    bool needsRemoved() const override;


};


#endif //DISCRETEMATHLAB3_LABEL_H
