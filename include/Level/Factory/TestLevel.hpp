#pragma once

#include "Level/Level.hpp"
#include "Object/Player.hpp"

namespace Level::Factory
{

void testLevel(Level* lvl,
			   sf::RenderWindow* window,
			   ResourceManager* resource);

}