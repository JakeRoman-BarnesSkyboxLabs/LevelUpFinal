#include "Precompiled.h"
#include "World/World.h"

#include "GameObjects/Rider.h"

const float World::mWallWidth = 2.5f;

World::World(sf::RenderWindow& renderWindow)
	: mRenderWindow(renderWindow)
{
	const float windowSizeX = static_cast<float>(mRenderWindow.getSize().x);
	const float windowSizeY = static_cast<float>(mRenderWindow.getSize().y);
	AABB horizontalBorder({ windowSizeX * 0.5f, mWallWidth * 0.5f }, windowSizeX, mWallWidth);
	AABB verticalBorder({ mWallWidth * 0.5f, windowSizeY * 0.5f }, mWallWidth, windowSizeY);

	sf::Color BorderfillColor(sf::Color::White);
	mCompletedWalls.push_back({ horizontalBorder, sf::Color::White });
	mCompletedWalls.push_back({ verticalBorder, sf::Color::White });
	horizontalBorder.mPos.y = windowSizeY - (mWallWidth * 0.5f);
	verticalBorder.mPos.x = windowSizeX - (mWallWidth * 0.5f);
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
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Space)
	{
		mGameActive = !mGameActive;
	}

	if (mGameActive)
	{
		for (auto& [player, trailingWall] : mHumanRiders)
		{
			player.HandleEvent(event);
		}
	}
}

void World::TickWorld(uint64_t deltaTime)
{
	if (mGameActive)
	{
		for (auto& [player, trailingWall] : mHumanRiders)
		{
			player.Tick(deltaTime);
			if (player.ChangedDirectionThisTick())
			{
				mCompletedWalls.emplace_back(trailingWall, player.GetColor());
			}
			const AABB& playerAABB = player.GetAABB();
			Vec2 trailingPoint = player.GetPos();
			Vec2 startingPos = player.GetMovementSegmentOrigin();
			const float halfWidth = playerAABB.GetWidth() * 0.5f;
			const float halfHeight = playerAABB.GetHeight() * 0.5f;
			switch (player.GetFacing())
			{
			case Utils::Direction::North:
				trailingPoint.y += halfHeight;
				startingPos.y += halfHeight;
				break;
			case Utils::Direction::South:
				trailingPoint.y -= halfHeight;
				startingPos.y -= halfHeight;
				break;
			case Utils::Direction::East:
				trailingPoint.x -= halfWidth;
				startingPos.x -= halfWidth;
				break;
			case Utils::Direction::West:
				trailingPoint.x += halfWidth;
				startingPos.x += halfWidth;
				break;
			}

			Vec2 distance = trailingPoint - startingPos;
			trailingWall.SetPosition((distance * 0.5f) + startingPos);
			if (distance.x + distance.y < 0)
			{
				distance *= -1.0f;
			}
			distance.x = std::max(distance.x, mWallWidth);
			distance.y = std::max(distance.y, mWallWidth);
			trailingWall.SetDimensions(distance);
		}
		for (auto& [cpu, trailingWall] : mCPURiders)
		{
			cpu.Tick(deltaTime);
		}
		HandleCollisions();
	}
}

void World::RenderWorld() const
{
	auto renderWall = [&renderWindow = mRenderWindow](std::pair<AABB, sf::Color> wall)
	{
		const sf::Vector2f renderDimensions = Vec2::toScreenVector(wall.first.GetWidth(), wall.first.GetHeight());
		const sf::Vector2f renderPosition = wall.first.mPos.toScreenVector();
		sf::RectangleShape wallShape(renderDimensions);
		wallShape.setPosition(renderPosition);
		wallShape.setOrigin({ wall.first.GetWidth() * 0.5f, wall.first.GetHeight() * 0.5f });
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

void World::HandleCollisions()
{
	for (auto& [player, trailingWall] : mHumanRiders)
	{
		if (RiderCrashed(player))
		{
			player.Die();
		}
	}
	for (const auto& [cpu, trailingWall] : mHumanRiders)
	{
		if (RiderCrashed(cpu))
		{

		}
	}
}
