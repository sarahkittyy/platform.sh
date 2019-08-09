#include "Object/Tilemap.hpp"

namespace Object
{

Tilemap::Tilemap(Props props)
	: Object(props),
	  mMapFile(props.get("/mapFile"_json_pointer)),
	  mAutotile(props.get("/autotile"_json_pointer))
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

void Tilemap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mMap, states);
}

void Tilemap::init()
{
	/// Should be rendered all the way in the back.
	setPriority(100);

	/// Possibly collideable.
	props().set({ { "collideable", true } });

	/// Initialize the map.
	mMap.load(&resource(), mMapFile, mAutotile);
}

bool Tilemap::isSolidAt(sf::Vector2i pos)
{
	return mMap.getTile(pos.x, pos.y) != 0;
}

}