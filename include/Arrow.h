//
// Created by ASUS on 03.01.2023.
//

#ifndef DISCRETEMATHLAB3_ARROW_H
#define DISCRETEMATHLAB3_ARROW_H

#include <Entity.h>
#include <Label.h>

class Arrow : public Entity {

private:
    int w;
    float lerpFactor;


private:
    Entity *start;
    Entity *end;
    size_t order1;
    size_t order2;
    Label text;
    bool is_oriented;
public:
    bool isOriented() const;

    void setIsOriented(bool isOriented);

private:
    bool rm_mark;
    sf::VertexArray line;
    sf::VertexArray arrow;
    float scale_arrow_factor = 20.f;
    sf::Font* font;
    void reinitArrow();

public:
    Arrow(Entity *targetStart, Entity *targetEnd, sf::Font &font,sf::View& view, const sf::Color &color = sf::Color::Black);

    int getW() const;

    void setW(int w);

    Entity *getStart() const;

    Entity *getEnd() const;

    sf::FloatRect getGlobalBounds() const override;

    void update(float dt) override;

    void markToRemove(bool remove) override;

    bool needsRemoved() const override;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    float getLerpFactor() const;

    void setLerpFactor(float lerpFactor);

    void setTextValidator(const std::function<bool(const char&)>& func);

    std::size_t getLabelTextSize();

    void setLabelString(const std::string &string);

    const std::string &getString();

    const std::string &getDestinationName() const;

    const std::string &getSourceName() const;
};


#endif //DISCRETEMATHLAB3_ARROW_H
