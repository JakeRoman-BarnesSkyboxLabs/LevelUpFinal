#include "Precompiled.h"

#include "GameObjects/ControlMap.h"

using Key = sf::Keyboard::Key;

const ControlLayout ControlLayout::ArrowKeys{ Key::Up, Key::Down, Key::Left, Key::Right };
const ControlLayout ControlLayout::WASD{ Key::W, Key::S, Key::A, Key::D };

ControlMap::ControlMap(const ControlLayout& layout)
{
	mControls[layout.mUp] = Utils::Direction::North;
	mControls[layout.mDown] = Utils::Direction::South;
	mControls[layout.mLeft] = Utils::Direction::West;
	mControls[layout.mRight] = Utils::Direction::East;
}

ControlMap::ControlMap(ControlMap&& other) noexcept
	: mControls(std::move(other.mControls))
{}

ControlMap& ControlMap::operator=(ControlMap&& other) noexcept
{
	mControls = std::move(other.mControls);
	return *this;
}

std::optional<Utils::Direction> ControlMap::getBoundDirection(sf::Keyboard::Key input) const
{
	auto boundDirection = mControls.find(input);
	return boundDirection != mControls.end() ? std::optional(boundDirection->second) : std::nullopt;
}
