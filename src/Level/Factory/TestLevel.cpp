#include "Level/Factory/TestLevel.hpp"

namespace Level::Factory
{

void testLevel(Level* lvl)
{
	using Object::Props;

	lvl->setDisplayText("deaths: 0");
	lvl->setTickSpeed(sf::seconds(0.6f));

	lvl->on("playerKilled", [lvl](const nlohmann::json& data) {
		lvl->setDisplayText("deaths: " + std::to_string(data.at("deathCount").get<int>()));
		lvl->reset();
	});

	// clang-format off
	lvl->addObject<Object::Player>("Player", Props()
		.set({
			{ "startPos", Object::Props::fromVector<float>(14.f, 1.f) }
		})
	);
	lvl->addObject(new Object::Tilemap(Props()));
	lvl->addObject(new Object::ArrowPlatform(Props()
		.set({
			{"pos", Props::fromVector<float>(11.f, 4.f)},
			{"dir", Object::ArrowPlatform::LEFT}
		})
	));
	lvl->addObject(new Object::ArrowPlatformEnd(Props()
		.set({
			{"pos", Props::fromVector<float>(10.f, 4.f)},
			{"turnTo", "right"}
		})
	));
	lvl->addObject(new Object::ArrowPlatformEnd(Props()
		.set({
			{"pos", Props::fromVector<float>(20.f, 4.f)},
			{"turnTo", "left"}
		})
	));
	// clang-format on
}

}