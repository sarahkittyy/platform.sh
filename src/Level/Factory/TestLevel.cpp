#include "Level/Factory/TestLevel.hpp"

namespace Level::Factory
{

void testLevel(Level* lvl,
			   sf::RenderWindow* window,
			   ResourceManager* resource)
{
	lvl->init(window, resource);
	lvl->addObject(new Object::Player({ 5.f, 4.f }));
	lvl->addObject(new Object::Tilemap("assets/maps/test_map.json", true));
}

}