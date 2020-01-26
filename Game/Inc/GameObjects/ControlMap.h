#pragma once
#include <optional>
#include "SFML/Window/Keyboard.hpp"

struct ControlLayout
{
	static const ControlLayout ArrowKeys;
	static const ControlLayout WASD;

	sf::Keyboard::Key mUp = sf::Keyboard::Key::Unknown;
	sf::Keyboard::Key mDown = sf::Keyboard::Key::Unknown;
	sf::Keyboard::Key mLeft = sf::Keyboard::Key::Unknown;
	sf::Keyboard::Key mRight = sf::Keyboard::Key::Unknown;
};

class ControlMap
{
	std::unordered_map<sf::Keyboard::Key, Utils::Direction> mControls;

public:
	ControlMap(const ControlLayout& layout);
	ControlMap(const ControlMap& other) = default;
	ControlMap(ControlMap&& other) noexcept;

	ControlMap& operator=(const ControlMap& other) = default;
	ControlMap& operator=(ControlMap&& other) noexcept;

	std::optional<Utils::Direction> getBoundDirection(sf::Keyboard::Key input) const;
};