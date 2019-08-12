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
	/// Init basic graphics
	Tilemap();

	/// Initialize the tilemap.
	void init(ResourceManager* resource);

	/**
	 * @brief Load the tilemap from a tiled json map file.
	 * 
	 * @param file The json map file
	 */
	void loadFromTiled(std::string file);

	/**
	 * @brief Load the tilemap normally.
	 * 
	 * @param mapSize The size of the map, in tiles
	 * @param tileSize The size of a tile, in pixels
	 * @param image The tilemap image.
	 */
	void load(sf::Vector2i mapSize,
			  sf::Vector2i tileSize,
			  std::string image);

	/**
	 * @brief If the map tilemap image is configured
	 * so that the bitmasked sum of all neighbors:
	 * 
	 *   1
	 * 2   4
	 *   8
	 * 
	 * .. is equal to it's position in the image,
	 * then this function will replace the tile on the map with
	 * the bitmasked version.
	 * 
	 * In short: overwrites tiles and smoothens out the map.
	 * 
	 */
	void autotile();

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

private:
	/// SFML draw() override.
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	/// The app resource manager.
	ResourceManager* mResource;

	/**
	 * @brief Reset all vertices, based on the contents of mTiles
	 * 
	 */
	void reloadVertices(bool autotile = false);

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