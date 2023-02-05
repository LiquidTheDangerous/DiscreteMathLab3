//
// Created by ASUS on 31.12.2022.
//

#ifndef DISCRETEMATHLAB3_ENTITY_H
#define DISCRETEMATHLAB3_ENTITY_H

#include <SFML/Graphics.hpp>
#include <Signal.h>

class Entity : public sf::Drawable,public sf::Transformable{
public:
    void setName(const std::string& name);
    const std::string& getName() const;
    Signal& getSignal(const std::size_t& id);
    virtual sf::FloatRect getGlobalBounds() const;
    virtual void update(float dt) = 0;
    virtual void markToRemove(bool remove) = 0;
    virtual bool needsRemoved()const = 0 ;
#pragma clang diagnostic push
#pragma ide diagnostic ignored "HidingNonVirtualFunction"
    virtual void setPosition(float x, float y);
    virtual void setPosition(const sf::Vector2f& );
    virtual void move(float x, float y);
    virtual void move(const sf::Vector2f& vect);
#pragma clang diagnostic pop

protected:
    friend class EntityEventDispatcherImpl;
    SignalContainer signals;
    std::string name;

private:
    using sf::Transformable::move;
    using sf::Transformable::setPosition;
};


#endif //DISCRETEMATHLAB3_ENTITY_H
