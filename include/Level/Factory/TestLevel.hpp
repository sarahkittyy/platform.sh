#pragma once

#include "Level/Level.hpp"
#include "Object/ArrowPlatform.hpp"
#include "Object/ArrowPlatformEnd.hpp"
#include "Object/Player.hpp"
#include "Object/Props.hpp"
#include "Object/Tilemap.hpp"

namespace Level::Factory
{

void testLevel(Level* lvl,
			   sf::RenderWindow* window,
			   ResourceManager* resource);

}