#include "Precompiled.h"
#include "SFML/Graphics.hpp"

#include "GameObjects/Rider.h"

int main()
{
	const sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(sf::VideoMode(1024, 768, desktopMode.bitsPerPixel), "virus.jif");

	Rider<RiderType::Player> rider;
	rider.SetPos({ 500, 300 });
	// TODO create a world to contain walls and riders

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Escape))
			{
				window.close();
			}
			// pass game events
			rider.HandleEvent(event);
		}

		// Tick game
		rider.Tick();

		window.clear();

		// Run game render calls
		rider.Render(window);

		window.display();
	}

	return 0;
}