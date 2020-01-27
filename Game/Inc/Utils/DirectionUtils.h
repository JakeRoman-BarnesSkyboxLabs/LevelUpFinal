#pragma once

namespace Utils
{

enum class Direction
{
	North,
	East,
	South,
	West
};

static std::unordered_map<Direction, Direction> OppositeDirection{
	{Direction::North, Direction::South},
	{Direction::South, Direction::North},
	{Direction::East, Direction::West},
	{Direction::West, Direction::East} };

constexpr float GetRotation(const Direction facing)
{
	switch (facing)
	{
	case Direction::North:
		return 180.0f;
		break;
	case Direction::East:
		return 90.0f;
		break;
	case Direction::South:
		return 0.0f;
		break;
	case Direction::West:
		return 270.0f;
		break;
	default:
		break;
	}
	return 0.0f;
}

inline Vec2 GetVector(const Direction facing)
{
	switch (facing)
	{
	case Direction::North:
		return { 0.0f, -1.0f };
		break;
	case Direction::East:
		return { 1.0f, 0.0f };
		break;
	case Direction::South:
		return { 0.0f, 1.0f };
		break;
	case Direction::West:
		return { -1.0f, 0.0f };
		break;
	default:
		break;
	}
	return { 0.0f, 0.0f };
}

}