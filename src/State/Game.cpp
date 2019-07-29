#include "State/Game.hpp"

namespace State
{

void Game::init()
{
	map.load(&resource(), "assets/maps/test_map.json", trues);
}

void Game::update()
{
	window().clear(sf::Color::Black);
	window().draw(map);

	window().display();
}

}