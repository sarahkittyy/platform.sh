#include "State/Game.hpp"

namespace State
{

void Game::init()
{
	mTestLevel.init(&resource(), "assets/maps/test_map.json", true);
}

void Game::update()
{
	mTestLevel.update();

	window().clear(sf::Color::Black);
	window().draw(mTestLevel);

	window().display();
}

}