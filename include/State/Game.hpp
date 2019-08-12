#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Level/Factory/Levels.hpp"
#include "Level/Level.hpp"
#include "Object/Object.hpp"
#include "ResourceManager.hpp"
#include "State/State.hpp"
#include <memory>

namespace State
{

/**
 * @brief Main game state.
 * 
 */
class Game : public State
{
public:
	/// Init the game, and use the test level.
	Game();
	/// Init the game with a pre-generated level.
	Game(Level::Level* level);

	void init();
	void update();

	void on(const sf::Event& event);

private:
	/// The level the game plays.
	std::shared_ptr<Level::Level> mLevel;
};

}