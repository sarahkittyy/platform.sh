#pragma once

#include <SFML/Graphics.hpp>
#include "GFX/TiledTilemap.hpp"
#include "Object/Object.hpp"
#include "ResourceManager.hpp"

namespace Object
{

/**
 * @brief Object wrapper for a static tilemap.
 * 
 */
class Tilemap : public Object
{
public:
	Tilemap(std::string mapFile, bool autotile = false);

	/// Initializes the tilemap graphic
	void init();

private:
	/// SFML draw() override.
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	/// The main tilemap graphical component.
	GFX::TiledTilemap mMap;

	std::string mMapFile;
	bool mAutotile;
};

}