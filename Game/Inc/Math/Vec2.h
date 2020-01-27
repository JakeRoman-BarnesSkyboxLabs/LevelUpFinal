#pragma once
#include <cstdint>

#include "SFML/System/Vector2.hpp"

struct Vec2
{
	int64_t x;
	int64_t y;

	Vec2 operator+(const Vec2& other) const { return { x + other.x, y + other.y }; }
	Vec2 operator-(const Vec2& other) const { return { x - other.x, y - other.y }; }
	Vec2 operator*(const Vec2& other) const { return { x * other.x, y * other.y }; }
	Vec2 operator/(const Vec2& other) const { return { x / other.x, y / other.y }; }
	Vec2 operator+=(const Vec2& other) { return { x += other.x, y += other.y }; }
	Vec2 operator-=(const Vec2& other) { return { x -= other.x, y -= other.y }; }
	Vec2 operator*=(const Vec2& other) { return { x *= other.x, y *= other.y }; }
	Vec2 operator/=(const Vec2& other) { return { x /= other.x, y /= other.y }; }

	Vec2 operator+(int64_t other) const { return { x + other, y + other }; }
	Vec2 operator-(int64_t other) const { return { x - other, y - other }; }
	Vec2 operator*(int64_t other) const { return { x * other, y * other }; }
	Vec2 operator/(int64_t other) const { return { x / other, y / other }; }

	bool operator==(const Vec2& other) const { return x == other.x && y == other.y; }
	bool operator!=(const Vec2& other) const { return x != other.x || y != other.y; }

	sf::Vector2f toScreenVector() const { return { static_cast<float>(x) / 100, static_cast<float>(y) / 100 }; }
	static sf::Vector2f toScreenVector(int64_t x, int64_t y) { return { static_cast<float>(x) / 100, static_cast<float>(y) / 100 }; }
};