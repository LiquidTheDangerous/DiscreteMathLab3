//
// Created by ASUS on 31.12.2022.
//

#ifndef DISCRETEMATHLAB3_ENTITYCONTAINER_H
#define DISCRETEMATHLAB3_ENTITYCONTAINER_H
#include <SFML/Graphics.hpp>
#include <Entity.h>
#include <list>
#include <memory>

enum signals{
  onLeftMouseClicked,

  signalsCount
};

class EntityContainer :
        public Entity {
public:
    using EntityPtr = std::shared_ptr<Entity>;
    void addEntity(const std::shared_ptr<Entity>& entity);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    virtual void handleEvent(const sf::Event& event);
private:
    std::list<EntityPtr> entities;
};




#endif //DISCRETEMATHLAB3_ENTITYCONTAINER_H
