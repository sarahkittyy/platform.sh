#include "State/Game.hpp"

namespace State
{

void Game::init()
{
	Level::Factory::testLevel(&mTestLevel, &window(), &resource());
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