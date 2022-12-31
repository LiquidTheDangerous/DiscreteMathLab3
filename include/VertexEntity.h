//
// Created by ASUS on 30.12.2022.
//

#ifndef DISCRETEMATHLAB3_VERTEXENTITY_H
#define DISCRETEMATHLAB3_VERTEXENTITY_H

#include <SFML/Graphics.hpp>
#include <Entity.h>
#include <MousePositionProvider.h>
#include <MousePositionProviderImpl.h>
#include <memory>
#include <utility>

class VertexEntity :
public Entity {
private:
    sf::CircleShape circle;
    sf::Text text;
    std::pair<float,float> mouse_offset;
    std::shared_ptr<MousePositionProvider> mpp;
    bool followsMouse = false;
public:
    explicit VertexEntity(sf::Font &font, const std::string &name);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void setMousePositionProvider(std::shared_ptr<MousePositionProvider> mpp);

    void followMouse(float offset_x, float offset_y);
    void unfollowMouse();

    void update(float dt) override;

    sf::FloatRect getGlobalBounds() const override;
    void setCircleFillColor(const sf::Color& color);
};


#endif //DISCRETEMATHLAB3_VERTEXENTITY_H
