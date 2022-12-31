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
    virtual void update(float dt);
protected:
    friend class MouseEventDispatcher;
    SignalContainer signals;
    std::string name;
};


#endif //DISCRETEMATHLAB3_ENTITY_H
