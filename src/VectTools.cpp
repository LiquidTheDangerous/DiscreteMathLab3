//
// Created by ASUS on 03.01.2023.
//

#include <VectTools.h>
#include <cmath>

float VectTools::dot(const sf::Vector2f& l, const sf::Vector2f& r)
{
	return l.x * r.x + l.y * r.y;
}

float VectTools::sqrMagnitude(const sf::Vector2f& vect)
{
	return vect.x * vect.x + vect.y * vect.y;
}

float VectTools::magnitude(const sf::Vector2f& vect)
{
    return std::sqrt(VectTools::sqrMagnitude(vect));
}

sf::Vector2f VectTools::leftNormal(const sf::Vector2f& vect)
{
	return {-vect.y,vect.x};
}

sf::Vector2f VectTools::rightNormal(const sf::Vector2f& vect)
{
	return {vect.y,-vect.x};
}

sf::Vector2f VectTools::normalize(const sf::Vector2f& vect)
{
	return vect / VectTools::magnitude(vect);
}

float VectTools::cosAngle(const sf::Vector2f& first, const sf::Vector2f& second)
{
	return VectTools::dot(first,second)/std::sqrt(VectTools::sqrMagnitude(first)*VectTools::sqrMagnitude(second));
}

float VectTools::angleDeg(const sf::Vector2f& first, const sf::Vector2f& second)
{
	return VectTools::angleRad(first, second) * 180.f / VectTools::PI;
}

float VectTools::angleRad(const sf::Vector2f& first, const sf::Vector2f& second)
{
	return std::acos(VectTools::cosAngle(first, second));
}