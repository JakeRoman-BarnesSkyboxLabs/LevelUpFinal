#include "Precompiled.h"
#include "SFML/Graphics.hpp"

#include "GameObjects/Rider.h"
#include "World/World.h"

void AddPlayers(World& world)
{
	Rider<RiderType::Player> rider1;
	rider1.SetPos({ 500 * 100, 300 * 100 });
	rider1.SetColor(sf::Color::Blue);

	//Rider<RiderType::Player> rider2(ControlLayout::WASD);
	//rider2.SetPos({ 100 * 100, 300 * 100 });
	//rider2.SetColor(sf::Color::Yellow);

	world.AddRider(rider1);
	//world.AddRider(rider2);
}

int main()
{
	const sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(sf::VideoMode(1024, 768, desktopMode.bitsPerPixel), "virus.jif");

	World world(window);
	AddPlayers(world);

	while (window.isOpen())
	{
		window.clear();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Escape))
			{
				window.close();
			}
			// pass game events
			world.HandleEvent(event);
		}

		// Tick game
		world.TickWorld();

		// Run game render calls
		world.RenderWorld();

		window.display();
	}

	return 0;
}