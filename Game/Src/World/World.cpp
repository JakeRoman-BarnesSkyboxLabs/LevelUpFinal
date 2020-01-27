#include "Precompiled.h"
#include "World/World.h"

#include "GameObjects/Rider.h"

World::World(sf::RenderWindow& renderWindow)
	: mRenderWindow(renderWindow)
{
	constexpr float wallWidth = 1.0f;
	const float windowSizeX = static_cast<float>(mRenderWindow.getSize().x);
	const float windowSizeY = static_cast<float>(mRenderWindow.getSize().y);
	AABB horizontalBorder(0.0f, 0.0f, windowSizeX, wallWidth);
	AABB verticalBorder(0.0f, 0.0f, wallWidth, windowSizeY);

	sf::Color BorderfillColor(sf::Color::White);
	mCompletedWalls.push_back({ horizontalBorder, sf::Color::White });
	mCompletedWalls.push_back({ verticalBorder, sf::Color::White });
	horizontalBorder.SetPosition(0.0f, windowSizeY - wallWidth);
	verticalBorder.SetPosition(windowSizeX - wallWidth, 0.0f);
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

void World::TickWorld(uint64_t deltaTime)
{
	for (auto& [player, trailingWall] : mHumanRiders)
	{
		player.Tick(deltaTime);
		if (player.ChangedDirectionThisTick())
		{
			mCompletedWalls.emplace_back(trailingWall, player.GetColor());
		}
		trailingWall.SetPosition(player.GetMovementSegmentOrigin());
		Vec2 dimensions = player.GetPos();
		dimensions -= player.GetMovementSegmentOrigin();
		if (dimensions.x == 0.0f)
		{
			dimensions.x = 1.0f;
		}
		if (dimensions.y == 0.0f)
		{
			dimensions.y = 1.0f;
		}

		trailingWall.SetDimensions(dimensions.x, dimensions.y);
	}
	for (auto& [cpu, trailingWall] : mCPURiders)
	{
		cpu.Tick(deltaTime);
	}
	HandleCollisions();
}

void World::RenderWorld() const
{
	auto renderWall = [&renderWindow = mRenderWindow](std::pair<AABB, sf::Color> wall)
	{
		const sf::Vector2f renderDimensions = Vec2::toScreenVector(wall.first.mWidth, wall.first.mHeight);
		const sf::Vector2f renderPosition = Vec2::toScreenVector(wall.first.mX, wall.first.mY);
		sf::RectangleShape wallShape(renderDimensions);
		wallShape.setPosition(renderPosition);
		wallShape.setFillColor(wall.second);
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
