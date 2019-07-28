#include "State/Game.hpp"

namespace State
{

void Game::init()
{
}

void Game::update()
{
	window().clear(sf::Color::Black);

	window().display();
}

}