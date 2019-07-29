#include "State/Game.hpp"

namespace State
{

void Game::init()
{
	mTestLevel = Level::Factory::testLevel(&window(), &resource());
	mTestLevel.start();
}

void Game::update()
{
	mTestLevel.update();

	window().clear(sf::Color::Black);
	window().draw(mTestLevel);

	window().display();
}

}