//
// Created by ASUS on 02.01.2023.
//

#include <iostream>
#include "Message.h"
#include "EntityEventDispatcherImpl.h"
void Message::update(float dt) {
    if (ttl > 0){
        this->ttl -= dt;
    }else{
        auto color = this->text.getFillColor();
        if (color.a - 9 > 0){
            color.a -= 9;
            this->text.setFillColor(color);
        }
        else{
            this->markToRemove(true);
        }
    }

}

void Message::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    if (this->visible){
        states.transform *= this->getTransform();
        target.draw(this->text,states);
    }
}

void Message::setVisible(bool visible) {
    this->visible = visible;
}

bool Message::isVisible() const {
    return this->visible;
}

void Message::markToRemove(bool remove) {
    if (remove){
        this->signals.emit(signals::onDelete,nullptr);
    }
    this->rm_flag = remove;
}

bool Message::needsRemoved() const {
    return this->rm_flag;
}

Message::Message(const sf::Font &font, const std::string &text, const float &ttl) {
    this->font = &font;
    this->text.setFillColor(sf::Color::Black);
    this->text.setString(text);
    this->text.setFont(*this->font);
    this->ttl = ttl;
    this->visible = true;
    this->rm_flag = false;
}
