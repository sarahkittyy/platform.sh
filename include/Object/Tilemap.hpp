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
	/**
	 * @brief Initialize the map.
	 * 
	 * @param mapFile The path to the map json file.
	 * @param autotile Whether or not to autotile the given map.
	 */
	Tilemap(std::string mapFile, bool autotile = false);

	Tilemap* create();
	Tilemap* clone();

	/// Initializes the tilemap graphic
	void init();

	/// Returns true for non-zero tiles at the given position.
	bool isSolidAt(sf::Vector2i pos);

private:
	/// SFML draw() override.
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	/// The main tilemap graphical component.
	GFX::TiledTilemap mMap;

	/// The path to the map json file.
	std::string mMapFile;
	/// Whether or not to autotile the map.
	bool mAutotile;
};

}