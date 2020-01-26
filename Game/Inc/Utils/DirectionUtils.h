#pragma once
#include "SFML/System/Vector2.hpp"

namespace Utils
{

enum class Direction
{
	North,
	East,
	South,
	West
};

constexpr float GetRotation(const Direction facing)
{
	switch (facing)
	{
	case Direction::North:
		return 0.0f;
		break;
	case Direction::East:
		return 90.0f;
		break;
	case Direction::South:
		return 180.0f;
		break;
	case Direction::West:
		return 270.0f;
		break;
	default:
		break;
	}
	return 0.0f;
}

inline sf::Vector2f GetVector(const Direction facing)
{
	switch (facing)
	{
	case Direction::North:
		return sf::Vector2f(0.0f, -1.0f);
		break;
	case Direction::East:
		return sf::Vector2f(1.0f, 0.0f);
		break;
	case Direction::South:
		return sf::Vector2f(0.0f, 1.0f);
		break;
	case Direction::West:
		return sf::Vector2f(-1.0f, 0.0f);
		break;
	default:
		break;
	}
	return sf::Vector2f(0.0f, 0.0f);
}

}