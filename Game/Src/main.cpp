#include "Precompiled.h"
#include "SFML/Graphics.hpp"
#include "SFML/System/Clock.hpp"
#include "SFML/System/Time.hpp"

#include "GameObjects/Rider.h"
#include "World/World.h"

void AddPlayers(World& world)
{
	Rider<RiderType::Player> rider1;
	rider1.SetPos({ 512, 738 });
	rider1.SetColor(sf::Color::Blue);

	Rider<RiderType::Player> rider2(ControlLayout::WASD);
	rider2.SetPos({ 512, 30 });
	rider2.SetColor(sf::Color::Yellow);
	rider2.SetDirection(Utils::Direction::South);

	world.AddRider(rider1);
	world.AddRider(rider2);
}

int main()
{
	const sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(sf::VideoMode(1024, 768, desktopMode.bitsPerPixel), "virus.jif");
	window.setFramerateLimit(60);
	sf::Clock clock;
	sf::Time deltaTime;

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
		deltaTime = clock.restart();
		world.TickWorld(deltaTime.asMilliseconds());

		// Run game render calls
		world.RenderWorld();

		window.display();
	}

	return 0;
}