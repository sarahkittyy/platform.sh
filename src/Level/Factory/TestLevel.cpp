#include "Level/Factory/TestLevel.hpp"

namespace Level::Factory
{

void testLevel(Level* lvl,
			   sf::RenderWindow* window,
			   ResourceManager* resource)
{
	lvl->init(window, resource, "assets/maps/test_map.json", true);
	lvl->addObject(new Object::Player({ 4, 3 }));
}

}