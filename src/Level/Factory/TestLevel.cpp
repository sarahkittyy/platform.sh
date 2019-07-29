#include "Level/Factory/TestLevel.hpp"

namespace Level::Factory
{

Level testLevel(sf::RenderWindow* window, ResourceManager* resource)
{
	Level lvl;
	lvl.init(window, resource, "assets/maps/test_map.json", true);
	lvl.addObject(new Object::Player({ 4, 3 }));
	return lvl;
}

}