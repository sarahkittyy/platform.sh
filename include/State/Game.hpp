#pragma once

#include <SFML/Graphics.hpp>
#include "GFX/TiledTilemap.hpp"
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

private:
	GFX::TiledTilemap map;
};

}