//
// Created by ASUS on 03.01.2023.
//

#ifndef DISCRETEMATHLAB3_ARROW_H
#define DISCRETEMATHLAB3_ARROW_H

#include <Entity.h>

class Arrow : public Entity {
    Entity *start;


private:
    Entity *end;
    size_t order1;
    size_t order2;
    bool rm_mark;
    sf::VertexArray line;
    sf::VertexArray arrow;
    float scale_arrow_factor = 20.f;

    void reinitArrow();

public:
    Arrow(Entity *targetStart, Entity *targetEnd, const sf::Color &color = sf::Color::Black);

    Entity *getStart() const;

    Entity *getEnd() const;

    sf::FloatRect getGlobalBounds() const override;

    void update(float dt) override;

    void markToRemove(bool remove) override;

    bool needsRemoved() const override;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;


};


#endif //DISCRETEMATHLAB3_ARROW_H
