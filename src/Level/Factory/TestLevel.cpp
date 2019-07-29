#include "Level/Factory/TestLevel.hpp"

namespace Level::Factory
{

Level::Level testLevel(sf::RenderWindow* window, ResourceManager* resource)
{
	Level lvl;
	lvl.init(window, resource, "assets/maps/test_map.json", true);
	return lvl;
}

}