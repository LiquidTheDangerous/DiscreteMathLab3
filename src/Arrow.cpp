//
// Created by ASUS on 03.01.2023.
//

#include "Arrow.h"
#include <EntityEventDispatcherImpl.h>
#include "VectTools.h"
#include "signals.hpp"
#include <iostream>

const sf::FloatRect Arrow::nullBody = sf::FloatRect(0, 0, 0, 0);

sf::FloatRect Arrow::getGlobalBounds() const {
    if (!this->show_text){
        return Arrow::nullBody;
    }
    return this->text.getGlobalBounds();
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
    if (remove) {
        start->getSignal(signals::onDelete).deleteSlot(order1);
        end->getSignal(signals::onDelete).deleteSlot(order2);
    }
    this->rm_mark = remove;
}

bool Arrow::needsRemoved() const {
    return this->rm_mark;
}

void Arrow::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= this->getTransform();
    target.draw(this->line, states);
    if (this->is_oriented) {
        target.draw(this->arrow, states);
    }
    if (this->show_text) {
        target.draw(this->text, states);
    }
}

Arrow::Arrow(Entity *targetStart, Entity *targetEnd, sf::Font &font, sf::View &view, const sf::Color &color) :
        line(sf::Lines, 2),
        arrow(sf::LineStrip, 3),
        rm_mark{false},
        start(targetStart),
        end(targetEnd),
        lerpFactor(0.5f),
        text(font, "", view, ""),
        is_oriented(true),
        show_text(true) {
    line[0] = sf::Vertex{targetStart->getPosition(), color};
    line[1] = sf::Vertex{targetEnd->getPosition(), color};

    order1 = targetStart->getSignal(signals::onDelete).addSlot([this, targetEnd](void *) {
        targetEnd->getSignal(signals::onDelete).deleteSlot(order2);
        this->markToRemove(true);
    });
    order2 = targetEnd->getSignal(signals::onDelete).addSlot([this, targetStart](void *) {
        targetStart->getSignal(signals::onDelete).deleteSlot(order1);
        this->markToRemove(true);
    });
    this->w = 1;
    this->text.setString("1");
    this->getSignal(signals::onLeftMouseClicked).addSlot([this](void *param) {
        this->text.emit(signals::onLeftMouseClicked, param);
    });

    this->getSignal(signals::onEnteredText).addSlot([this](void *param) {
        this->text.emit(signals::onEnteredText, param);
    });
    this->text.getSignal(signals::onEndEditingText).addSlot([this](void *param) {
        this->emit(signals::onEndEditingText, param);
    });
    this->font = &font;
    this->reinitArrow();
}

void Arrow::reinitArrow() {
    auto mid_point = VectTools::Lerp(this->line[0].position, this->line[1].position, this->lerpFactor);
    auto vect = VectTools::normalize(this->line[1].position - this->line[0].position);
    auto l_norm = VectTools::leftNormal(vect);
    auto r_norm = VectTools::rightNormal(vect);
    this->text.Transformable::setPosition((this->line[0].position + this->line[1].position) / 2.f);

    arrow[0].position = l_norm * this->scale_arrow_factor + mid_point;
    arrow[1].position = vect * this->scale_arrow_factor + mid_point;
    arrow[2].position = r_norm * this->scale_arrow_factor + mid_point;
    arrow[0].color = sf::Color::Black;
    arrow[1].color = sf::Color::Black;
    arrow[2].color = sf::Color::Black;
}

Entity *Arrow::getStart() const {
    return start;
}

Entity *Arrow::getEnd() const {
    return end;
}

int Arrow::getW() const {
    return w;
}

void Arrow::setW(int w) {
    this->text.setString(std::to_string(w));
    Arrow::w = w;
}

float Arrow::getLerpFactor() const {
    return lerpFactor;
}

void Arrow::setLerpFactor(float lerpFactor) {
    Arrow::lerpFactor = lerpFactor;
}

std::size_t Arrow::getLabelTextSize() {
    return this->text.getString().size();
}

void Arrow::setLabelString(const std::string &string) {
    this->text.setString(string);
}

void Arrow::setTextValidator(const std::function<bool(const char &)> &func) {
    this->text.setTextValidator(func);
}

const std::string &Arrow::getString() {
    return this->text.getString();
}

const std::string &Arrow::getDestinationName() const {
    return this->end->getName();
}

const std::string &Arrow::getSourceName() const {
    return this->start->getName();
}

bool Arrow::isOriented() const {
    return is_oriented;
}

void Arrow::setIsOriented(bool isOriented) {
    is_oriented = isOriented;
}

bool Arrow::isShowText() const {
    return show_text;
}

void Arrow::setShowText(bool showText) {
    show_text = showText;
}

void Arrow::setLineColor(const sf::Color &color) {
    this->line[0].color = color;
    this->line[1].color = color;
}

sf::Color Arrow::getLineColor() const {
    return this->line[0].color;
}

