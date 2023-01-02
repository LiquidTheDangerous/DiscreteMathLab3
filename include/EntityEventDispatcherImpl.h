//
// Created by ASUS on 31.12.2022.
//

#ifndef DISCRETEMATHLAB3_ENTITYEVENTDISPATCHERIMPL_H
#define DISCRETEMATHLAB3_ENTITYEVENTDISPATCHERIMPL_H

#include <SFML/Graphics.hpp>
#include <Entity.h>
#include <list>
#include <memory>
#include <MousePositionProvider.h>
#include <EntityEventDispatcher.h>

enum signals {
    onLeftMouseClicked,
    onRightMouseClicked,
    onMiddleMouseClicked,
    onLeftMouseReleased,
    onRightMouseReleased,
    onMiddleMouseReleased,
    onMouseEntered,
    onMouseLeaved,
    onEnteredText,
    signalsCount
};

class EntityEventDispatcherImpl :
        public EntityEventDispatcher {
public:

    void setMousePositionProvider(std::shared_ptr<MousePositionProvider> MousePositionProvider) override;

    using EntityPtr = std::shared_ptr<Entity>;

    void addEntity(const std::shared_ptr<Entity> &entity) override;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    void handleEvent(const sf::Event &event) override;

    void update(float dt) override;

private:
    std::shared_ptr<MousePositionProvider> mousePositionProvider;
    std::list<EntityPtr> entities;
    std::size_t e_id = -1;
};


#endif //DISCRETEMATHLAB3_ENTITYEVENTDISPATCHERIMPL_H
