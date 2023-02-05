//
// Created by ASUS on 02.01.2023.
//

#ifndef DISCRETEMATHLAB3_MESSAGE_H
#define DISCRETEMATHLAB3_MESSAGE_H

#include <string>
#include <Entity.h>
#include <GuiComponent.hpp>

class Message : public GUIComponent{
private:
    const sf::Font *font;
    sf::Text text;
    float ttl;
    bool visible;
    bool rm_flag;

public:
    Message(const sf::Font &font, const std::string &text, const float &ttl, sf::View &view);

    void update(float dt) override;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    void setVisible(bool visible);

    bool isVisible() const;

    void markToRemove(bool remove) override;

    bool needsRemoved() const override;

};


#endif //DISCRETEMATHLAB3_MESSAGE_H
