#pragma once
#include "Precompiled.h"
#include "GameObjects/Rider.h"

class World {
	std::vector<Rider<RiderType::Player>> mHumanRiders;
	std::vector<Rider<RiderType::CPU>> mCPURiders;
	std::vector<sf::RectangleShape> mWalls;

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
	void TickWorld();
	void RenderWorld() const;

private:
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
