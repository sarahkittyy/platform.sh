#pragma once

#include "Level/Level.hpp"
#include "Object/Player.hpp"

namespace Level::Factory
{

Level testLevel(sf::RenderWindow* window, ResourceManager* resource);

}