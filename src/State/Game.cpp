#include "State/Game.hpp"

namespace State
{

void Game::init()
{
	mTestLevel.init(&window(), &resource());

	Level::Factory::testLevel(&mTestLevel);
	mTestLevel.start();
}

void Game::on(const sf::Event& event)
{
	switch (event.type)
	{
	default:
		break;
	case sf::Event::Resized:
		mTestLevel.emit("windowResized", { { "x", event.size.width }, { "y", event.size.height } });
		break;
	}
}

void Game::update()
{
	mTestLevel.update();

	window().clear(sf::Color::Black);
	window().draw(mTestLevel);

	window().display();
}

}