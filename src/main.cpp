#include <SFML/Graphics.hpp>
#include "ResourceManager.hpp"
#include "State/Game.hpp"
#include "State/Machine.hpp"

int main()
{
	// Create the main app vars.
	sf::RenderWindow window(sf::VideoMode(800, 600), "platform.sh");
	ResourceManager resource;

	// Init the state machine.
	State::Machine sm(new State::Game(), &window, &resource);

	// Start the main game loop
	while (window.isOpen())
	{
		// Event polling
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			default:
				break;
			case sf::Event::Closed:
				window.close();
				break;
			}

			// Pass the event to the state machine.
			sm.on(event);
		}

		// Update the state machine.
		sm.update();
	}

	return 0;
}