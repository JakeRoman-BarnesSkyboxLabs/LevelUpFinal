#pragma once
#include "GameObjects/ControlMap.h"
#include "Utils/DirectionUtils.h"
#include "SFML/Graphics.hpp"

namespace sf
{
	class RenderWindow;
}

enum class RiderType
{
	Player,
	CPU
};

template<RiderType type>
class Rider
{
	ControlMap mController;
	sf::Vector2f mPos;
	sf::RectangleShape mAABB = { 1.0f,1.5f };
	Utils::Direction mFacing = Utils::Direction::North;

public:

	Rider(ControlLayout controls = ControlLayout::ArrowKeys);
	Rider(const ControlMap& customControls);
	Rider(const Rider& other) = delete;
	Rider(Rider&& other) noexcept; 

	void Initialize();

	Rider& operator=(const Rider& other) = delete;
	Rider& operator=(Rider&& other) noexcept;

	void SetPos(sf::Vector2f pos) { mPos = std::move(pos); }

	void HandleEvent(const sf::Event& event);
	void Tick(/*world?*/);
	void Render(sf::RenderWindow& window);
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
	, mAABB({ 1.0f,1.5f })
{
	Initialize();
}

template<RiderType type>
Rider<type>::Rider(Rider&& other) noexcept
	: mController(std::move(other.mController))
	, mPos(std::move(other.mPos))
	, mAABB(std::move(other.mAABB))
	, mFacing(std::move(other.mFacing))
{
}

template<RiderType type>
inline void Rider<type>::Initialize()
{
	mAABB.setOrigin(mAABB.getSize().x * 0.5f, mAABB.getSize().y * 0.5f);
	mAABB.setScale(10.0f, 10.0f);
}

template<RiderType type>
Rider<type>& Rider<type>::operator=(Rider&& other) noexcept
{
	mController = std::move(other.mController);
	mPos = std::move(other.mPos);
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
			if (direction.has_value())
			{
				mFacing = direction.value();
			}
		}
	}
}

template<RiderType type>
void Rider<type>::Tick()
{
	if constexpr (type == RiderType::CPU)
	{
		// TODO Look for possible direction changes
	}

	// move rider in facing direction
	constexpr float moveSpeed = 0.05f;
	mPos += (Utils::GetVector(mFacing) * moveSpeed);

	// TODO spawn trail
	// TODO check for collisions
}

template<RiderType type>
void Rider<type>::Render(sf::RenderWindow& window)
{
	sf::RectangleShape shape(mAABB);
	shape.setPosition(mPos);
	shape.setRotation(Utils::GetRotation(mFacing));

	shape.setOutlineColor(sf::Color::White);
	shape.setOutlineThickness((shape.getSize().x + shape.getSize().y) * 0.5f * 0.1f);
	shape.setFillColor(sf::Color::Blue);

	window.draw(shape);
}