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
    sf::Text additionalLabel;
    std::pair<float, float> mouse_offset;
    std::shared_ptr<MousePositionProvider> mpp;
    sf::Color defaultColor;
    sf::Color hoverColor;
    bool showAdditionalLabel;
    bool followsMouse = false;
    bool rm_flag;

public:
    explicit VertexEntity(sf::Font &font, const std::string &name, std::shared_ptr<MousePositionProvider> mpp);

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    void setMousePositionProvider(std::shared_ptr<MousePositionProvider> mpp);

    void followMouse(float offset_x, float offset_y);

    void unfollowMouse();

    bool isFollowedMouse() const;

    void update(float dt) override;

    void setDefaultColor(const sf::Color &color);

    sf::Color getDefaultColor() const;

    void setHoverColor(const sf::Color &color);

    sf::Color getHoverColor() const;

    sf::FloatRect getGlobalBounds() const override;

    void setCircleFillColor(const sf::Color &color);

    void markToRemove(bool remove) override;

    bool needsRemoved() const override;

    void setAdditionalLabelString(const std::string &string);

    std::string getAdditionalLabelString() const;

    bool isShowAdditionalLabel() const;

    void setShowAdditionalLabel(bool showAdditionalLabel);
};


#endif //DISCRETEMATHLAB3_VERTEXENTITY_H
