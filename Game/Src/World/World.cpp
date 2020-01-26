#include "Precompiled.h"
#include "World/World.h"

#include "GameObjects/Rider.h"

World::World(sf::RenderWindow& renderWindow)
	: mRenderWindow(renderWindow)
{
	constexpr int64_t wallWidth = 1;
	const int64_t windowSizeX = static_cast<int64_t>(mRenderWindow.getSize().x) * 100;
	const int64_t windowSizeY = static_cast<int64_t>(mRenderWindow.getSize().y) * 100;
	AABB horizontalBorder(0, 0, windowSizeX, wallWidth);
	AABB verticalBorder(0, 0, wallWidth, windowSizeY);

	sf::Color BorderfillColor(sf::Color::White);
	mCompletedWalls.push_back({ horizontalBorder, sf::Color::White });
	mCompletedWalls.push_back({ verticalBorder, sf::Color::White });
	horizontalBorder.SetPosition(0, windowSizeY - wallWidth);
	verticalBorder.SetPosition(windowSizeX - wallWidth, 0);
	mCompletedWalls.push_back({ horizontalBorder, sf::Color::White });
	mCompletedWalls.push_back({ verticalBorder, sf::Color::White });
}

int World::AddRider(const Rider<RiderType::Player>& other)
{
	const int riderID = static_cast<int>(mHumanRiders.size());
	mHumanRiders.push_back({ other, AABB() });
	return riderID;
}

int World::AddRider(Rider<RiderType::Player>&& other)
{
	const int riderID = static_cast<int>(mHumanRiders.size());
	mHumanRiders.push_back({ other, AABB() });
	return riderID;
}

void World::HandleEvent(const sf::Event& event)
{
	for (auto& [player, trailingWall] : mHumanRiders)
	{
		player.HandleEvent(event);
	}
}

void World::TickWorld()
{
	for (auto& [player, trailingWall] : mHumanRiders)
	{
		player.Tick();
		if (player.ChangedDirectionThisTick())
		{
			mCompletedWalls.emplace_back(trailingWall, player.GetColor());
		}
		trailingWall.SetPosition(player.GetMovementSegmentOrigin());
		Vec2 dimensions = player.GetPos();
		dimensions.x -= player.GetMovementSegmentOrigin().x;
		dimensions.y -= player.GetMovementSegmentOrigin().y;
		trailingWall.SetDimensions(dimensions.x, dimensions.y);
	}
	for (auto& [cpu, trailingWall] : mCPURiders)
	{
		cpu.Tick();
	}
	HandleCollisions();
}

void World::RenderWorld() const
{
	auto renderWall = [&renderWindow = mRenderWindow](std::pair<AABB, sf::Color> wall)
	{
		sf::Vector2f renderDimensions = Vec2::toScreenVector(wall.first.mWidth, wall.first.mHeight);
		sf::Vector2f renderPosition = Vec2::toScreenVector(wall.first.mX, wall.first.mY);
		sf::RectangleShape wallShape(renderDimensions);
		wallShape.setPosition(renderPosition);
		wallShape.setOutlineThickness(1.0f);
		wallShape.setOutlineColor(wall.second);
		renderWindow.draw(wallShape);
	};

	for (const auto& [player, trailingWall] : mHumanRiders)
	{
		player.Render(mRenderWindow);
		renderWall({ trailingWall, player.GetColor() });
	}
	for (const auto& [cpu, trailingWall] : mCPURiders)
	{
		cpu.Render(mRenderWindow);
		renderWall({ trailingWall, cpu.GetColor() });
	}
	std::for_each(mCompletedWalls.begin(), mCompletedWalls.end(), renderWall);
}

void World::HandleCollisions() const
{
	for (const auto& [player, trailingWall] : mHumanRiders)
	{
		if (RiderCrashed(player))
		{
			// TODO explosions or something, idk
			mRenderWindow.close();
		}
	}
	for (const auto& [cpu, trailingWall] : mHumanRiders)
	{
		if (RiderCrashed(cpu))
		{

		}
	}
}
