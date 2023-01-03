//
// Created by ASUS on 03.01.2023.
//

#include "Arrow.h"
#include <EntityEventDispatcherImpl.h>
#include "VectTools.h"

sf::FloatRect Arrow::getGlobalBounds() const {
    return Entity::getGlobalBounds();
}

void Arrow::update(float dt) {
    if (start == nullptr || end == nullptr) {
        this->markToRemove(true);
        return;
    }
    line[0].position = start->getPosition();
    line[1].position = end->getPosition();
    this->reinitArrow();
}

void Arrow::markToRemove(bool remove) {
    this->rm_mark = remove;
}

bool Arrow::needsRemoved() const {
    return this->rm_mark;
}

void Arrow::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= this->getTransform();
    target.draw(this->line, states);
    target.draw(this->arrow,states);
}

Arrow::Arrow(Entity *targetStart, Entity *targetEnd, const sf::Color &color) :
        line(sf::Lines, 2),
        arrow(sf::LineStrip,3),
        rm_mark{false},
        start(targetStart),
        end(targetEnd) {
    line[0] = sf::Vertex{targetStart->getPosition(), color};
    line[1] = sf::Vertex{targetEnd->getPosition(), color};

    order1 = targetStart->getSignal(signals::onDelete).addSlot([this,targetEnd](void *) {
        targetEnd->getSignal(signals::onDelete).deleteSlot(order2);
        this->markToRemove(true);
    });
    order2 = targetEnd->getSignal(signals::onDelete).addSlot([this,targetStart](void *) {
        targetStart->getSignal(signals::onDelete).deleteSlot(order1);
        this->markToRemove(true);
    });
    this->reinitArrow();
}

void Arrow::reinitArrow() {
    auto mid_point = (this->line[0].position + this->line[1].position)/2.f;
    auto vect = VectTools::normalize(this->line[1].position - this->line[0].position);
    auto l_norm = VectTools::leftNormal(vect);
    auto r_norm = VectTools::rightNormal(vect);

    arrow[0].position = l_norm*this->scale_arrow_factor + mid_point;
    arrow[1].position = vect*this->scale_arrow_factor + mid_point;
    arrow[2].position = r_norm*this->scale_arrow_factor + mid_point;
    arrow[0].color = sf::Color::Black;
    arrow[1].color = sf::Color::Black;
    arrow[2].color = sf::Color::Black;
//    arrow.
}

