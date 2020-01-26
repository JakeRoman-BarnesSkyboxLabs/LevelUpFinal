#include "Precompiled.h"
#include "World/World.h"

#include "GameObjects/Rider.h"

World::World(sf::RenderWindow& renderWindow)
	: mRenderWindow(renderWindow)
{
	constexpr float wallWidth = 1.0f;
	const float windowSizeX = static_cast<float>(mRenderWindow.getSize().x);
	const float windowSizeY = static_cast<float>(mRenderWindow.getSize().y);
	sf::RectangleShape horizontalBorder({ windowSizeX, wallWidth });
	horizontalBorder.setFillColor(sf::Color::White);
	sf::RectangleShape verticalBorder({ wallWidth, windowSizeY });
	verticalBorder.setFillColor(sf::Color::White);

	mWalls.push_back(horizontalBorder);
	mWalls.push_back(verticalBorder);
	horizontalBorder.setPosition(0.0f, windowSizeY - wallWidth);
	verticalBorder.setPosition(windowSizeX - wallWidth, 0.0f);
	mWalls.push_back(horizontalBorder);
	mWalls.push_back(verticalBorder);
}

int World::AddRider(const Rider<RiderType::Player>& other)
{
	const int riderID = static_cast<int>(mHumanRiders.size());
	mHumanRiders.push_back(other);
	return riderID;
}

int World::AddRider(Rider<RiderType::Player>&& other)
{
	const int riderID = static_cast<int>(mHumanRiders.size());
	mHumanRiders.push_back(other);
	return riderID;
}

void World::HandleEvent(const sf::Event& event)
{
	for (auto& player : mHumanRiders)
	{
		player.HandleEvent(event);
	}
}

void World::TickWorld()
{
	for (auto& player : mHumanRiders)
	{
		player.Tick();
	}
	for (auto& cpu : mCPURiders)
	{
		cpu.Tick();
	}
}

void World::RenderWorld() const
{
	for (const auto& player : mHumanRiders)
	{
		player.Render(mRenderWindow);
	}
	for (const auto& cpu : mCPURiders)
	{
		cpu.Render(mRenderWindow);
	}
	for (const auto& wall : mWalls)
	{
		mRenderWindow.draw(wall);
	}
}
