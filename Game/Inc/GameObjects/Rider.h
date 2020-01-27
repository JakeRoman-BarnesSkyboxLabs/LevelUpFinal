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
	static constexpr int64_t kBaseMoveSpeed = 30;

	std::string mName;
	ControlMap mController;
	sf::Color mColor;
	AABB mAABB;
	Vec2 mPosLast;
	Utils::Direction mFacing = Utils::Direction::North;

	Vec2 mPosDirectionChange;

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

	const sf::Color& GetColor() const { return mColor; }
	const AABB& GetAABB() const { return mAABB; };
	const Vec2& GetPos() const { return { mAABB.mX, mAABB.mY }; };
	const Vec2& GetMovementSegmentOrigin() const { return mPosDirectionChange; }

	bool ChangedDirectionThisTick() const { return mPosDirectionChange == mPosLast; }

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
	mAABB.mHeight = 1000;
	mAABB.mWidth = 750;
	mPosLast = {mAABB.mX, mAABB.mY};
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
void Rider<type>::HandleEvent(const sf::Event& event)
{
	if constexpr (type == RiderType::Player)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			const std::optional<Utils::Direction> direction = mController.getBoundDirection(event.key.code);
			if (direction.has_value() && direction.value() != mFacing && direction.value() != Utils::OppositeDirection[mFacing])
			{
				mFacing = direction.value();
				mPosDirectionChange = GetPos();
			}
		}
	}
}

template<RiderType type>
void Rider<type>::Tick(uint64_t deltaTime)
{
	if constexpr (type == RiderType::CPU)
	{
		// TODO Look for possible direction changes
	}

	// move rider in facing direction
	mPosLast = GetPos();
	Vec2 moveDelta = Utils::GetVector(mFacing) * kBaseMoveSpeed * deltaTime;
	mAABB.Move(moveDelta.x, moveDelta.y);
}

template<RiderType type>
void Rider<type>::Render(sf::RenderWindow& window) const
{
	sf::RectangleShape shape(Vec2::toScreenVector(mAABB.mWidth, mAABB.mHeight));

	shape.setPosition(Vec2::toScreenVector(mAABB.mX, mAABB.mY));
	shape.setOrigin(Vec2::toScreenVector(mAABB.mWidth / 2, mAABB.mHeight / 2));
	shape.setRotation(Utils::GetRotation(mFacing));

	shape.setFillColor(mColor);
	shape.setOutlineColor(sf::Color::White);
	shape.setOutlineThickness((shape.getSize().x + shape.getSize().y) * 0.5f * 0.1f);

	sf::CircleShape center(1000.0f);
	center.setOrigin(Vec2::toScreenVector(mAABB.mWidth / 2, mAABB.mHeight / 2));
	center.setPosition(GetPos().toScreenVector());
	center.setFillColor(sf::Color::Green);

	window.draw(shape);
	window.draw(center);
}