#include <SFML/Graphics.hpp>
#include "ResourceManager.hpp"
#include "State/Machine.hpp"
#include "State/Menu.hpp"
#include "imgui/imgui-SFML.h"
#include "imgui/imgui.h"

int main()
{
	// Create the main app vars.
	sf::RenderWindow window(sf::VideoMode(800, 600), "platform.sh");
	ResourceManager resource;

	// Initialize SFML.
	ImGui::SFML::Init(window);

	// Init the state machine.
	State::Machine sm(new State::Menu(), &window, &resource);

	// For updating ImGui.
	sf::Clock appClock;

	// Start the main game loop
	while (window.isOpen())
	{
		// Event polling
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Process the event with ImGui
			ImGui::SFML::ProcessEvent(event);

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

		// Update ImGui
		ImGui::SFML::Update(window, appClock.restart());

		// Update the state machine.
		sm.update();

		// Assert imgui has ended.
		ImGui::EndFrame();
	}

	return 0;
}