//
// Created by ASUS on 30.12.2022.
//

#include <VertexEntity.h>
#include <EntityEventDispatcherImpl.h>
#include <MousePositionProvider.h>
#include <iostream>

void VertexEntity::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= this->getTransform();
    target.draw(this->circle, states);
    target.draw(this->text, states);
}

VertexEntity::VertexEntity(sf::Font &font, const std::string &name, std::shared_ptr<MousePositionProvider> mpp) {
    this->text.setFont(font);
    this->setName(name);
    this->text.setFillColor(sf::Color::Black);
    this->text.setString(this->name);
    this->circle.setRadius(20.f);
    this->circle.move(-20.f, -20.f);
    this->circle.setFillColor(sf::Color::Red);
    this->mpp = std::move(mpp);
    this->rm_flag = false;
    this->getSignal(signals::onLeftMouseClicked).addSlot([this](void*) {

        this->followMouse(0, 0);
    });
    this->getSignal(signals::onLeftMouseReleased).addSlot([this](void*) {
        this->unfollowMouse();
    });
    this->getSignal(signals::onMouseEntered).addSlot([this](void*) {
        auto name = this->getName();
        this->setCircleFillColor(sf::Color::Black);
    });
    this->getSignal(signals::onMouseLeaved).addSlot([this](void*) {
        this->setCircleFillColor(sf::Color::Red);
    });
}

sf::FloatRect VertexEntity::getGlobalBounds() const {
    return this->getTransform().transformRect(this->circle.getGlobalBounds());

}

void VertexEntity::setCircleFillColor(const sf::Color &color) {
    this->circle.setFillColor(color);
}

void VertexEntity::setMousePositionProvider(std::shared_ptr<MousePositionProvider> mpp) {
    this->mpp = std::move(mpp);
}

void VertexEntity::followMouse(float offset_x, float offset_y) {
    this->followsMouse = true;
    this->mouse_offset = std::make_pair(offset_x,offset_y);
}

void VertexEntity::unfollowMouse() {
    this->followsMouse = false;
}

void VertexEntity::update(float dt) {
//    std::cout << this->getName() << std::endl;
//    std::cout << this->getPosition().x << ' ' << this->getPosition().y << std::endl;
    if (!this->followsMouse)
        return;
    auto mp = this->mpp->getMousePosition();
    this->setPosition(mp.first + this->mouse_offset.first,mp.second + this->mouse_offset.second);
}

bool VertexEntity::isFollowedMouse() const {
    return this->followsMouse;
}

void VertexEntity::markToRemove(bool remove) {
    this->signals.emit(signals::onDelete,nullptr);
    this->rm_flag = remove;
}

bool VertexEntity::needsRemoved() const {
    return this->rm_flag;
}


