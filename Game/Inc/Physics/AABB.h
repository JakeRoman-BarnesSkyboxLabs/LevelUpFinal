#pragma once
#include "Math/Vec2.h"

class AABB
{
public:
	int64_t mX = 0;
	int64_t mY = 0;
	int64_t mWidth = 1;
	int64_t mHeight = 1;

public:
	AABB() = default;
	AABB(int64_t x, int64_t y, int64_t width = 1, int64_t height = 1) : mX(x), mY(y), mWidth(width), mHeight(height) {}

	void SetPosition(int64_t x, int64_t y) { mX = x; mY = y; }
	void SetPosition(Vec2 pos) { SetPosition(pos.x, pos.y); }
	void SetDimensions(int64_t x, int64_t y) { mWidth = x; mHeight = y; }
	void Move(int64_t x, int64_t y) { mX += x; mY += y; }

	Vec2 GetCenter() const { return { mX + (mWidth / 2), mY + (mHeight / 2) }; }
	int64_t GetLeftExtend() const;
	int64_t GetRightExtend() const;
	int64_t GetTopExtend() const;
	int64_t GetBottomExtend() const;
	bool Collides(const AABB& other) const;
};