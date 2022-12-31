//
// Created by ASUS on 31.12.2022.
//

#ifndef DISCRETEMATHLAB3_MOUSEEVENTDISPATCHER_H
#define DISCRETEMATHLAB3_MOUSEEVENTDISPATCHER_H

#include <SFML/Graphics.hpp>
#include <Entity.h>
#include <list>
#include <memory>
#include <MousePositionProvider.h>

enum signals {
    onLeftMouseClicked,
    onRightMouseClicked,
    onMiddleMouseClicked,
    onLeftMouseReleased,
    onRightMouseReleased,
    onMiddleMouseReleased,
    onMouseEntered,
    onMouseLeaved,
    signalsCount
};

class MouseEventDispatcher :
        public Entity {
public:

    void setMousePositionProvider(std::shared_ptr<MousePositionProvider> MousePositionProvider);

    using EntityPtr = std::shared_ptr<Entity>;

    void addEntity(const std::shared_ptr<Entity> &entity);

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    virtual void handleEvent(const sf::Event &event);

    void update(float dt) override;

private:
    std::shared_ptr<MousePositionProvider> mousePositionProvider;
    std::list<EntityPtr> entities;
    std::size_t e_id = -1;
};


#endif //DISCRETEMATHLAB3_MOUSEEVENTDISPATCHER_H
