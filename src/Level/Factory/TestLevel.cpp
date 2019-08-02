#include "Level/Factory/TestLevel.hpp"

namespace Level::Factory
{

void testLevel(Level* lvl,
			   sf::RenderWindow* window,
			   ResourceManager* resource)
{
	lvl->init(window, resource);
	lvl->addObject(new Object::Player({ 10.f, 2.f }));
	lvl->addObject(new Object::Tilemap("assets/maps/test_map.json", true));
	lvl->addObject(new Object::ArrowPlatform({ 20.f, 4.f }, Object::ArrowPlatform::LEFT));
	lvl->addObject(new Object::ArrowPlatformEnd({ 4.f, 4.f }, 90));
}

}