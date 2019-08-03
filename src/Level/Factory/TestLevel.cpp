#include "Level/Factory/TestLevel.hpp"

namespace Level::Factory
{

void testLevel(Level* lvl,
			   sf::RenderWindow* window,
			   ResourceManager* resource)
{
	lvl->init(window, resource);
	lvl->setDisplayText("deaths: 0");
	lvl->setTickSpeed(sf::seconds(0.6f));

	lvl->on("playerKilled", [lvl](const nlohmann::json& data) {
		lvl->setDisplayText("deaths: " + std::to_string(data.at("deathCount").get<int>()));
		lvl->reset();
	});

	lvl->addObject(new Object::Player({ 14.f, 1.f }));
	lvl->addObject(new Object::Tilemap("assets/maps/test_map.json", true));
	lvl->addObject(new Object::ArrowPlatform({ 11.f, 4.f }, Object::ArrowPlatform::LEFT));
	lvl->addObject(new Object::ArrowPlatformEnd({ 10.f, 4.f }, "right"));
	lvl->addObject(new Object::ArrowPlatformEnd({ 20.f, 4.f }, "left"));
}

}