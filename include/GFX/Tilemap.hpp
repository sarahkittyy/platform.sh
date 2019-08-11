#pragma once

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "ResourceManager.hpp"
#include "nlohmann/json.hpp"

namespace GFX
{

/**
 * @brief Tiled-compatible static tilemap.
 * 
 */
class Tilemap : public sf::Drawable, public sf::Transformable
{
public:
	/// Init
	Tilemap();

	/**
	 * @brief Load the tilemap from a tiled json map file.
	 * 
	 * @param resource The resource manager.
	 * @param file The json map file
	 * @param autotile If true, all tiles will be considered equal, and the texture will be assumed to be able to handle autotiling. The map will then be autotiled.
	 */
	void load(ResourceManager* resource, std::string file, bool autotile = false);

	/**
	 * @brief Get the tile ID at a given position.
	 * 
	 * @param x The x position.
	 * @param y The y position.
	 * @return int 0 for nothing, otherwise the tile ID.
	 */
	int getTile(int x, int y) const;

	/// Get the map size, in tiles.
	sf::Vector2i getMapSize() const;
	/// Get the tile size, in pixels.
	sf::Vector2i getTileSize() const;
	/// Get the raw Tiled map json data.
	const nlohmann::json& getRawMapData() const;

private:
	/// SFML draw() override.
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	/**
	 * @brief Reset and init vertices
	 * 
	 * @param autotile If true, all non-0 tiles will be converted to bitmasked autotiles.
	 * 
	 *   1
	 * 2   4
	 *   8
	 * 
	 * Ex: Tile 3 should have surrounding tiles on faces 1 and 2.
	 */
	void loadVertices(bool autotile);

	/**
	 * @brief Get the bitmask sum of all non-zero neighbors surrounding a tile.
	 * @see loadVertices
	 * 
	 * @param index The index of the tile.
	 * @return unsigned char The bitmask sum of all non-zero neighbors.
	 */
	unsigned char getNeighborBitmask(int index);

	/// The main vertex array.
	sf::VertexArray mVertices;
	/// Pointer to the tilemap texture.
	sf::Texture* mTexture;
	/// The tiled json data loaded.
	nlohmann::json mData;

	/// The actual tile integer values loaded from Tiled
	std::vector<int> mTiles;
	/// The map size
	sf::Vector2i mMapSize;
	/// The tile size.
	sf::Vector2i mTileSize;
};

}