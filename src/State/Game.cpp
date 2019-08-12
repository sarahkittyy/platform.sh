#include "State/Game.hpp"

namespace State
{

Game::Game()
	: mFromEditor(false)
{
}

Game::Game(std::shared_ptr<Level::Level> level, bool fromEditor)
	: mFromEditor(fromEditor)
{
	mLevel = level;
}

Game::~Game()
{
	mLevel->stop();
	mLevel->reset();
}

void Game::init()
{
	// Use the test level if no level is specified.
	if (!mLevel)
	{
		mLevel.reset(new Level::Level());
		mLevel->init(&window(), &resource());
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
	// Editor-redirect specific functionality.
	if (mFromEditor)
	{
		// Main function to implement editor functionality.
		fromEditor();
	}
	mLevel->update();

	window().clear(sf::Color::Black);
	window().draw(*mLevel);

	window().display();
}

void Game::fromEditor()
{
	// SFML Keyboard Shortcuts
	using Key	= sf::Keyboard;
	auto& KeyHit = Key::isKeyPressed;

	if (KeyHit(Key::Escape))
	{
		changeState(new Edit(mLevel));
	}
}

}