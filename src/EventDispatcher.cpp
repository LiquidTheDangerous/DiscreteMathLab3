//
// Created by ASUS on 30.12.2022.
//

#include <EventDispatcher.h>


void EventDispatcher::handleEvent(const sf::Event& event, const sf::Time& dt)
{
	auto iter1 = this->bindsEvent.find(event.type);

	if (iter1 != this->bindsEvent.end())
		iter1->second.operator()(event, dt);

	if (event.type == sf::Event::KeyPressed) {

		auto iter2 = this->bindsEventKeyCodePressed.find(event.key.code);

		if (iter2 == this->bindsEventKeyCodePressed.end())
			return;

		iter2->second.operator()(dt);
	}

	if (event.type == sf::Event::KeyReleased) {

		auto iter2 = this->bindsEventKeyCodeReleased.find(event.key.code);

		if (iter2 == this->bindsEventKeyCodeReleased.end())
			return;

		iter2->second.operator()(dt);
	}

	if (event.type == sf::Event::MouseButtonReleased) {

		auto iter2 = this->bindsMouseReleased.find(event.mouseButton.button);

		if (iter2 == this->bindsMouseReleased.end())
			return;

		iter2->second.operator()(dt);
	}

}



void EventDispatcher::handleInput(const sf::Time& dt)
{
	for (auto& c_pair : this->bindsKeyboard) {

		auto& keyCode = c_pair.first;

		auto& action = c_pair.second;

		if (sf::Keyboard::isKeyPressed(keyCode))
			action(dt);
	}
	for (auto& c_pair : this->bindsMousePressed) {

		auto& buttonCode = c_pair.first;

		auto& action = c_pair.second;

		if (sf::Mouse::isButtonPressed(buttonCode))
			action(dt);
	}


}


void EventDispatcher::addListenerOnKey(sf::Keyboard::Key key, const std::function<void(const sf::Time&)>& func)
{
	this->bindsKeyboard.emplace(key, func);
}

void EventDispatcher::addListenerOnEvent(sf::Event::EventType eventType, const std::function<void(const sf::Event&, const sf::Time&)>& func)
{
	this->bindsEvent.emplace(eventType, func);
}

void EventDispatcher::addListenerOnKeyPressedEvent(sf::Keyboard::Key key, const std::function<void(const sf::Time&)>& func)
{
	this->bindsEventKeyCodePressed.emplace(key, func);
}

void EventDispatcher::addListenerOnKeyReleasedEvent(sf::Keyboard::Key key, const std::function<void(const sf::Time&)>& func)
{
	this->bindsEventKeyCodeReleased.emplace(key, func);
}

void EventDispatcher::addListenerOnMousePressed(sf::Mouse::Button mouseButton, const std::function<void(const sf::Time&)>& func)
{
	this->bindsMousePressed.emplace(mouseButton, func);
}

void EventDispatcher::addListenerOnMouseReleasedEvent(sf::Mouse::Button mouseButton, const std::function<void(const sf::Time&)>& func)
{
	this->bindsMouseReleased.emplace(mouseButton, func);
}

void EventDispatcher::removeListenerOnMouseReleasedEvent(sf::Mouse::Button mouseButton)
{
	auto iter = this->bindsMouseReleased.find(mouseButton);
	if (iter == this->bindsMouseReleased.end())
		return;
	this->bindsMouseReleased.erase(iter);
}

void EventDispatcher::removeListenerOnKeyReleasedEvent(sf::Keyboard::Key key, const std::function<void(const sf::Time&)>&)
{
	auto iter = this->bindsEventKeyCodeReleased.find(key);
	if (iter == this->bindsEventKeyCodeReleased.end())
		return;
	this->bindsEventKeyCodeReleased.erase(iter);
}

void EventDispatcher::removeListenerOnKeyPressedEvent(sf::Keyboard::Key key)
{
	auto iter = this->bindsEventKeyCodePressed.find(key);
	if (iter == this->bindsEventKeyCodePressed.end())
		return;
	this->bindsEventKeyCodePressed.erase(iter);
}

void EventDispatcher::removeListenerOnMouse(sf::Mouse::Button mouseButton)
{
	auto iter = this->bindsMousePressed.find(mouseButton);
	if (iter == this->bindsMousePressed.end())
		return;
	this->bindsMousePressed.erase(iter);
}

void EventDispatcher::removeListenerOnKey(sf::Keyboard::Key key)
{
	auto iter = this->bindsKeyboard.find(key);
	if (iter == this->bindsKeyboard.end())
		return;

	this->bindsKeyboard.erase(iter);
}

void EventDispatcher::removeListenerOnEvent(sf::Event::EventType event)
{
	auto iter = this->bindsEvent.find(event);

	if (iter == this->bindsEvent.end())
		return;

	this->bindsEvent.erase(iter);
}
