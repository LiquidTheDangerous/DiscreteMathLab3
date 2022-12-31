//
// Created by ASUS on 30.12.2022.
//

#ifndef DISCRETEMATHLAB3_VERTEXENTITY_H
#define DISCRETEMATHLAB3_VERTEXENTITY_H

#include <SFML/Graphics.hpp>
#include <Entity.h>

class VertexEntity :
public Entity {
private:
    sf::CircleShape circle;
    sf::Text text;

public:
    explicit VertexEntity(sf::Font &font, const std::string &name);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    sf::FloatRect getGlobalBounds() const override;
    void setCircleFillColor(const sf::Color& color);
};


#endif //DISCRETEMATHLAB3_VERTEXENTITY_H
