#pragma once
#include "GameObjects/Rider.h"
#include "Physics/AABB.h"

class World {
	std::vector<std::pair<Rider<RiderType::Player>, AABB>> mHumanRiders;
	std::vector<std::pair<Rider<RiderType::CPU>, AABB>> mCPURiders;
	std::vector<std::pair<AABB, sf::Color>> mCompletedWalls;

	sf::RenderWindow& mRenderWindow;

public:
	World(sf::RenderWindow& renderWindow);

	template<RiderType type>
	int AddRider();
	int AddRider(const Rider<RiderType::Player>& other);
	int AddRider(Rider<RiderType::Player>&& other);

	template<RiderType type>
	Rider<type> GetRider(unsigned int riderID);

	void HandleEvent(const sf::Event& event);
	void TickWorld(uint64_t deltaTime);
	void RenderWorld() const;

private:
	template<RiderType type>
	bool RiderCrashed(const Rider<type>& rider) const;
	void HandleCollisions() const;
};

template<RiderType type>
int World::AddRider()
{
	if constexpr (type == RiderType::CPU)
	{
		const int riderID = mCPURiders.size();
		mCPURiders.emplace_back();
	}
	else
	{
		return AddRider(Rider<RiderType::Player>());
	}
}

template<RiderType type>
inline Rider<type> World::GetRider(unsigned int riderID)
{
	if constexpr (type == RiderType::CPU)
	{
		return mCPURiders[riderID];
	}
	else
	{
		return mHumanRiders[riderID];
	}
}

template<RiderType type>
inline bool World::RiderCrashed(const Rider<type>& rider) const
{
	for (const auto& [testRider, testWall] : mHumanRiders)
	{
		if constexpr (type == RiderType::Player)
		{
			if (rider.GetColor() == testRider.GetColor())
			{
				continue;
			}
		}
		if (rider.GetCollisionAABB().Collides(testWall))
		{
			return true;
		}
	}
	for (const auto& [cpu, testWall] : mCPURiders)
	{
		if constexpr (type == RiderType::CPU)
		{
			if (rider.GetColor() == cpu.GetColor())
			{
				continue;
			}
		}
		if (rider.GetCollisionAABB().Collides(testWall))
		{
			return true;
		}
	}
	for (const auto& [wall, color] : mCompletedWalls)
	{
		if (rider.GetCollisionAABB().Collides(wall))
		{
			return true;
		}
	}

	return false;
}
