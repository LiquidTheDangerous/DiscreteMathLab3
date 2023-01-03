//
// Created by ASUS on 03.01.2023.
//

#ifndef DISCRETEMATHLAB3_VECTTOOLS_H
#define DISCRETEMATHLAB3_VECTTOOLS_H
#include <SFML/Graphics.hpp>
namespace VectTools
{
	float dot(const sf::Vector2f& l, const sf::Vector2f& r);
	float sqrMagnitude(const sf::Vector2f& vect);
	float magnitude(const sf::Vector2f& vect);
	sf::Vector2f leftNormal(const sf::Vector2f& vect);
	sf::Vector2f rightNormal(const sf::Vector2f& vect);
	sf::Vector2f normalize(const sf::Vector2f& vect);
	float cosAngle(const sf::Vector2f& first, const sf::Vector2f& second);
	float angleRad(const sf::Vector2f& first, const sf::Vector2f& second);
	float angleDeg(const sf::Vector2f& first, const sf::Vector2f& second);

	constexpr float PI = 3.1415926535897932f;
};

#endif //DISCRETEMATHLAB3_VECTTOOLS_H
