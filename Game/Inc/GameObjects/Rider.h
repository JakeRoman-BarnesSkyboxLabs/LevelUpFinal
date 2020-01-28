#pragma once
#include "GameObjects/ControlMap.h"
#include "Physics/AABB.h"
#include "Utils/DirectionUtils.h"
#include "SFML/Graphics.hpp"

enum class RiderType
{
	Player,
	CPU
};

template<RiderType type>
class Rider
{
	static constexpr float kBaseMoveSpeed = 0.2f;

	std::string mName;
	ControlMap mController;
	sf::Color mColor;
	AABB mAABB;
	Vec2 mPosLast;
	Vec2 mPosDirectionChange;
	Utils::Direction mFacing = Utils::Direction::North;
	bool mIsAlive = true;

public:

	explicit Rider(ControlLayout controls = ControlLayout::ArrowKeys);
	Rider(const ControlMap& customControls);
	Rider(const Rider& other) = default;
	Rider(Rider&& other) noexcept = default;
	~Rider() = default;

	void Initialize();

	Rider& operator=(const Rider& other) = delete;
	Rider& operator=(Rider&& other) noexcept;

	void SetControls(const ControlLayout& controls) { mController = std::move(ControlMap(controls)); }
	void SetPos(Vec2 pos) { mAABB.SetPosition(pos.x, pos.y); mPosLast = pos; mPosDirectionChange = pos; }
	void SetColor(sf::Color color) { mColor = color; }
	void SetDirection(Utils::Direction facing) { mFacing = facing; }

	const sf::Color& GetColor() const { return mColor; }
	const AABB& GetAABB() const { return mAABB; };
	Utils::Direction GetFacing() const { return mFacing; };
	AABB GetCollisionAABB() const;
	const Vec2& GetPos() const { return mAABB.mPos; };
	const Vec2& GetMovementSegmentOrigin() const { return mPosDirectionChange; }

	bool ChangedDirectionThisTick() const { return mPosDirectionChange == mPosLast; }

	void Die() { mIsAlive = false; }

	void HandleEvent(const sf::Event& event);
	void Tick(uint64_t deltaTime);
	void Render(sf::RenderWindow& window) const;
};

template<RiderType type>
Rider<type>::Rider(ControlLayout controls)
	: mController(controls)
{
	Initialize();
}

template<RiderType type>
Rider<type>::Rider(const ControlMap& customControls)
	: mController(customControls)
{
	Initialize();
}

template<RiderType type>
inline void Rider<type>::Initialize()
{
	mAABB.SetDimensions(7.5f, 10.0f);
	mPosLast = mAABB.mPos;
	mPosDirectionChange = mPosLast;
}

template<RiderType type>
Rider<type>& Rider<type>::operator=(Rider&& other) noexcept
{
	mController = std::move(other.mController);
	mFacing = std::move(other.mFacing);

	return *this;
}

template<RiderType type>
inline AABB Rider<type>::GetCollisionAABB() const
{
	AABB collisionBox(mAABB);
	if (ChangedDirectionThisTick())
	{
		return collisionBox;
	}
	const Vec2 pos = GetPos();
	const Vec2 travelDistThisTick = pos - mPosLast;
	collisionBox.Grow(travelDistThisTick);
	return collisionBox;
}

template<RiderType type>
void Rider<type>::HandleEvent(const sf::Event& event)
{
	if (!mIsAlive)
	{
		return;
	}
	if constexpr (type == RiderType::Player)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			const std::optional<Utils::Direction> direction = mController.getBoundDirection(event.key.code);
			if (direction.has_value() && direction.value() != mFacing && direction.value() != Utils::OppositeDirection[mFacing])
			{
				Utils::Direction oldFacing = mFacing;
				mFacing = direction.value();
				mAABB.SetDimensions(mAABB.GetHeight(), mAABB.GetWidth());
				if (mFacing == Utils::Direction::North || mFacing == Utils::Direction::South)
				{
					mAABB.mPos += Utils::GetVector(mFacing) * mAABB.GetHeight() * 0.5f;
					mAABB.mPos -= Utils::GetVector(oldFacing) * mAABB.GetHeight() * 0.5f;
				}
				else
				{
					mAABB.mPos += Utils::GetVector(mFacing) * mAABB.GetWidth() * 0.5f;
					mAABB.mPos -= Utils::GetVector(oldFacing) * mAABB.GetWidth() * 0.5f;
				}
				mPosDirectionChange = GetPos();
			}
		}
	}
}

template<RiderType type>
void Rider<type>::Tick(uint64_t deltaTime)
{
	if (!mIsAlive)
	{
		return;
	}
	if constexpr (type == RiderType::CPU)
	{
		// TODO Look for possible direction changes
	}

	// move rider in facing direction
	mPosLast = GetPos();
	Vec2 moveDelta = Utils::GetVector(mFacing) * kBaseMoveSpeed * static_cast<float>(deltaTime);
	mAABB.Move(moveDelta.x, moveDelta.y);
}

template<RiderType type>
void Rider<type>::Render(sf::RenderWindow& window) const
{
	constexpr float outlineScale = 0.01f;
	if (!mIsAlive)
	{
		sf::CircleShape shape((mAABB.GetWidth() * 0.25f) + (mAABB.GetHeight() * 0.25f));
		const float radius = shape.getRadius();
		shape.setPosition(mAABB.mPos.toScreenVector());
		shape.setOrigin({ radius, radius });
		shape.setFillColor(mColor);
		shape.setOutlineColor(sf::Color::White);
		shape.setOutlineThickness(radius * radius * 3.14f * outlineScale);

		window.draw(shape);
		return;
	}
	sf::RectangleShape shape(Vec2::toScreenVector(mAABB.GetWidth(), mAABB.GetHeight()));

	shape.setPosition(mAABB.mPos.toScreenVector());
	shape.setOrigin({ mAABB.GetWidth() * 0.5f, mAABB.GetHeight() * 0.5f });

	shape.setFillColor(mColor);
	shape.setOutlineColor(sf::Color::White);
	shape.setOutlineThickness((shape.getSize().x * shape.getSize().y) * outlineScale);

	window.draw(shape);
}