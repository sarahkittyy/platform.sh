#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Level/Factory/Levels.hpp"
#include "Level/Level.hpp"
#include "Object/Object.hpp"
#include "ResourceManager.hpp"
#include "State/State.hpp"

namespace State
{

/**
 * @brief Main game state.
 * 
 */
class Game : public State
{
public:
	void init();
	void update();

	void on(const sf::Event& event);

private:
	Level::Level mTestLevel;
};

}