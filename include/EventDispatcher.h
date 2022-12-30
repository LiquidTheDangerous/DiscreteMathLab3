//
// Created by ASUS on 30.12.2022.
//

#ifndef DISCRETEMATHLAB3_EVENTDISPATCHER_H
#define DISCRETEMATHLAB3_EVENTDISPATCHER_H

#include <map>
#include <SFML/Graphics.hpp>

class EventDispatcher {
public:

    void handleEvent(const sf::Event &event, const sf::Time &dt);

    void handleInput(const sf::Time &dt);

    void addListenerOnKey(sf::Keyboard::Key, const std::function<void(const sf::Time &)>&);

    void addListenerOnEvent(sf::Event::EventType, const std::function<void(const sf::Event &, const sf::Time &)>&);

    void addListenerOnKeyPressedEvent(sf::Keyboard::Key, const std::function<void(const sf::Time &)>&);

    void addListenerOnKeyReleasedEvent(sf::Keyboard::Key, const std::function<void(const sf::Time &)>&);

    void addListenerOnMousePressed(sf::Mouse::Button, const std::function<void(const sf::Time &)>&);

    void addListenerOnMouseReleasedEvent(sf::Mouse::Button, const std::function<void(const sf::Time &)>&);

    void removeListenerOnMouseReleasedEvent(sf::Mouse::Button);

    void removeListenerOnKeyReleasedEvent(sf::Keyboard::Key, const std::function<void(const sf::Time &)>&);

    void removeListenerOnKeyPressedEvent(sf::Keyboard::Key);

    void removeListenerOnMouse(sf::Mouse::Button);

    void removeListenerOnKey(sf::Keyboard::Key);

    void removeListenerOnEvent(sf::Event::EventType);

private:
    std::map<sf::Mouse::Button, std::function<void(const sf::Time &)>> bindsMousePressed;

    std::map<sf::Keyboard::Key, std::function<void(const sf::Time &)>> bindsKeyboard;

    std::map<sf::Keyboard::Key, std::function<void(const sf::Time &)>> bindsEventKeyCodePressed;

    std::map<sf::Keyboard::Key, std::function<void(const sf::Time &)>> bindsEventKeyCodeReleased;

    std::map<sf::Event::EventType, std::function<void(const sf::Event &, const sf::Time &)>> bindsEvent;

    std::map<sf::Mouse::Button, std::function<void(const sf::Time &)>> bindsMouseReleased;
};


#endif //DISCRETEMATHLAB3_EVENTDISPATCHER_H
