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
	sf::RectangleShape mAABB;
	Utils::Direction mFacing = Utils::Direction::North;

public:

	explicit Rider(ControlLayout controls = ControlLayout::ArrowKeys);
	Rider(const ControlMap& customControls);
	Rider(const Rider& other) = default;
	Rider(Rider&& other) noexcept; 

	void Initialize();

	Rider& operator=(const Rider& other) = delete;
	Rider& operator=(Rider&& other) noexcept;

	void SetControls(const ControlLayout& controls) { mController = std::move(ControlMap(controls)); }
	void SetPos(sf::Vector2f pos) { mAABB.setPosition(std::move(pos)); }
	void SetColor(sf::Color color) { mAABB.setFillColor(std::move(color)); }

	void HandleEvent(const sf::Event& event);
	void Tick();
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
Rider<type>::Rider(Rider&& other) noexcept
	: mController(std::move(other.mController))
	, mAABB(std::move(other.mAABB))
	, mFacing(std::move(other.mFacing))
{
}

template<RiderType type>
inline void Rider<type>::Initialize()
{
	mAABB.setSize({ 1.0f,1.5f });
	mAABB.setOrigin(mAABB.getSize().x * 0.5f, mAABB.getSize().y * 0.5f);
	mAABB.setScale(10.0f, 10.0f);
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
	mAABB.setPosition(mAABB.getPosition() + (Utils::GetVector(mFacing) * moveSpeed));

	// TODO spawn trail
	// TODO check for collisions
}

template<RiderType type>
void Rider<type>::Render(sf::RenderWindow& window) const
{
	sf::RectangleShape shape(mAABB);
	shape.setRotation(Utils::GetRotation(mFacing));

	shape.setOutlineColor(sf::Color::White);
	shape.setOutlineThickness((shape.getSize().x + shape.getSize().y) * 0.5f * 0.1f);

	window.draw(shape);
}