//
// Created by ASUS on 31.12.2022.
//

#ifndef DISCRETEMATHLAB3_ENTITYEVENTDISPATCHER_H
#define DISCRETEMATHLAB3_ENTITYEVENTDISPATCHER_H

#include <Entity.h>
#include <MousePositionProvider.h>
#include <memory>

class EntityEventDispatcher : public Entity{
public:
    virtual void setMousePositionProvider(std::shared_ptr<MousePositionProvider> MousePositionProvider) = 0;

    virtual void addEntity(const std::shared_ptr<Entity> &entity) = 0;

    virtual void handleEvent(const sf::Event &event) = 0;



    virtual std::size_t getEntitiesSize() const = 0;
};


#endif //DISCRETEMATHLAB3_ENTITYEVENTDISPATCHER_H
