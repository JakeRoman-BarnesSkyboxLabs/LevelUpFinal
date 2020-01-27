#pragma once
#include <cstdint>

#include "SFML/System/Vector2.hpp"

struct Vec2
{
	float x;
	float y;

	Vec2 operator+(const Vec2& other) const { return { x + other.x, y + other.y }; }
	Vec2 operator-(const Vec2& other) const
	{
		float xResult = x - other.x;
		float yResult = y - other.y;
		return Vec2{ xResult, yResult };
	}
	Vec2 operator*(const Vec2& other) const { return { x * other.x, y * other.y }; }
	Vec2 operator/(const Vec2& other) const { return { x / other.x, y / other.y }; }
	Vec2 operator+=(const Vec2& other) { return { x += other.x, y += other.y }; }
	Vec2 operator-=(const Vec2& other) { return { x -= other.x, y -= other.y }; }
	Vec2 operator*=(const Vec2& other) { return { x *= other.x, y *= other.y }; }
	Vec2 operator/=(const Vec2& other) { return { x /= other.x, y /= other.y }; }

	Vec2 operator+(float other) const { return { x + other, y + other }; }
	Vec2 operator-(float other) const { return { x - other, y - other }; }
	Vec2 operator*(float other) const { return { x * other, y * other }; }
	Vec2 operator/(float other) const { return { x / other, y / other }; }

	bool operator==(const Vec2& other) const { return x == other.x && y == other.y; }
	bool operator!=(const Vec2& other) const { return x != other.x || y != other.y; }

	sf::Vector2f toScreenVector() const { return { static_cast<float>(x), static_cast<float>(y) }; }
	static sf::Vector2f toScreenVector(float x, float y) { return { static_cast<float>(x), static_cast<float>(y) }; }
};