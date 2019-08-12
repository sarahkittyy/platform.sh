#include "Object/Tilemap.hpp"

namespace Object
{

Tilemap::Tilemap(Props props)
	: Object(props)
{
}

Tilemap* Tilemap::create(Props props)
{
	return new Tilemap(props);
}

Tilemap* Tilemap::clone()
{
	return new Tilemap(initialProps());
}

GFX::Tilemap& Tilemap::map()
{
	return mMap;
}

nlohmann::json Tilemap::serialize() const
{
	return nlohmann::json({ { "map", mMap.serialize() } });
}

void Tilemap::deserialize(const nlohmann::json& data)
{
	mMap.deserialize(data["map"]);
}

const std::string Tilemap::name() const
{
	return "Tilemap";
}

const std::string Tilemap::icon() const
{
	return "assets/icons/tilemap.png";
}

void Tilemap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mMap, states);
}

void Tilemap::init()
{
	/// Should be rendered all the way in the back.
	setPriority(100);

	/// Possibly collideable.
	props().set({ { "collideable", true } }).set({ { "tilemap", true } });

	/// Initialize the map.
	mMap.init(&resource());
}

bool Tilemap::isSolidAt(sf::Vector2i pos)
{
	return mMap.getTile(pos.x, pos.y) != 0;
}

}