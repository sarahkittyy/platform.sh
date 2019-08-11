#include "State/Game.hpp"

namespace State
{

void Game::init()
{
	std::ofstream file("leveltest.json");
	Level::Factory::testLevel(&mTestLevel, &window(), &resource());
	file << mTestLevel.serialize();
	file.close();
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