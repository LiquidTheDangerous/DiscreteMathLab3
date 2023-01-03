//
// Created by ASUS on 31.12.2022.
//

#include "Label.h"
#include "EntityEventDispatcherImpl.h"

sf::FloatRect Label::getGlobalBounds() const {
    if (this->text.getString().getSize() == 0){
        return this->getTransform().transformRect(sf::FloatRect(0,0,100,100));
    }
    return this->getTransform().transformRect(this->text.getGlobalBounds());
}

void Label::update(float dt) {

}

void Label::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= this->getTransform();
    target.draw(this->text, states);
    if (this->isSelected()) {
        target.draw(this->line, states);
    }
}

Label::Label(sf::Font &font, const std::string &placeholder, const std::string &text) :
        line(sf::Lines, 2) {
    this->text.setFont(font);
    this->text.setFillColor(sf::Color::Black);
    this->setPlaceholder(placeholder);
    this->setString(text);
    this->getSignal(signals::onLeftMouseClicked).addSlot([this](void *) {
        auto selected = this->isSelected();
        selected ^= true;
        this->setSelected(selected);
    });

    this->getSignal(signals::onEnteredText).addSlot([this](void *param) {
        if (this->isSelected()) {
            auto event = static_cast<sf::Event *>(param);
            auto symbol = static_cast<char>(event->text.unicode);
            if (symbol == '\b') {
                auto str = this->getString();
                this->setString(str.substr(0, str.size() - 1));
                return;
            }
            if (symbol == '\r') {
                this->setSelected(false);
                this->signals.emit(signals::onEndEditingText,nullptr);
                return;
            }
            this->setString(this->getString() + symbol);
//            this->reinitLine();
        }
    });
}

void Label::setString(const std::string &text) {
    this->string_text = text;
    if (this->string_text.empty()){
        this->text.setString(this->placeholder);
    }
    else{
        this->text.setString(text);
    }
    this->reinitLine();
}

const std::string &Label::getString() const {
    return this->string_text;
}

bool Label::isSelected() const {
    return this->selected;
}

void Label::setSelected(bool selected) {
    this->selected = selected;
}

void Label::setPlaceholder(const std::string &placeholder) {
    this->placeholder = placeholder;
}

const std::string &Label::getPlaceholder() const {
    return this->placeholder;
}

void Label::reinitLine() {
    auto bounds = this->text.getGlobalBounds();
    this->line[0] = sf::Vertex(sf::Vector2f(bounds.left, bounds.top + bounds.height), sf::Color::Black);
    this->line[1] = sf::Vertex(sf::Vector2f(bounds.left + bounds.width, bounds.top + bounds.height),
                               sf::Color::Black);
}

void Label::markToRemove(bool remove) {
    this->signals.emit(signals::onDelete,nullptr);
    this->rm_flag = remove;
}

bool Label::needsRemoved()const {
    return this->rm_flag;
}
