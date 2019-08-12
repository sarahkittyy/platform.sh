#include "State/Game.hpp"

namespace State
{

Game::Game()
{
}

Game::Game(Level::Level* level)
{
	mLevel.reset(level);
	mLevel->start();
}

void Game::init()
{
	// Use the test level if no level is specified.
	if (!mLevel)
	{
		Level::Factory::testLevel(mLevel.get());
	}

	mLevel->start();
}

void Game::on(const sf::Event& event)
{
	switch (event.type)
	{
	default:
		break;
	case sf::Event::Resized:
		mLevel->emit("windowResized", { { "x", event.size.width }, { "y", event.size.height } });
		break;
	}
}

void Game::update()
{
	mLevel->update();

	window().clear(sf::Color::Black);
	window().draw(*mLevel);

	window().display();
}

}