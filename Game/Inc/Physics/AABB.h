#pragma once
#include "Math/Vec2.h"

class AABB
{
public:
	float mX = 0.0f;
	float mY = 0.0f;
	float mWidth = 1.0f;
	float mHeight = 1.0f;

public:
	AABB() = default;
	AABB(float x, float y, float width = 1.0f, float height = 1.0f) : mX(x), mY(y), mWidth(width), mHeight(height) {}

	void SetPosition(float x, float y) { mX = x; mY = y; }
	void SetPosition(Vec2 pos) { SetPosition(pos.x, pos.y); }
	void SetDimensions(float x, float y) { mWidth = x; mHeight = y; }
	void Grow(Vec2 increase);
	void Move(float x, float y) { mX += x; mY += y; }

	Vec2 GetCenter() const { return { mX + (mWidth * 0.5f), mY + (mHeight * 0.5f) }; }
	int64_t GetLeftExtend() const;
	int64_t GetRightExtend() const;
	int64_t GetTopExtend() const;
	int64_t GetBottomExtend() const;
	bool Collides(const AABB& other) const;
};