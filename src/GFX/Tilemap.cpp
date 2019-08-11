#include "GFX/Tilemap.hpp"

namespace GFX
{

Tilemap::Tilemap()
{
	mVertices.setPrimitiveType(sf::Quads);
}

void Tilemap::load(ResourceManager* resource,
				   std::string json_file,
				   bool autotile)
{
	namespace fs = std::filesystem;

	std::ifstream file(json_file);
	if (!file)
	{
		throw std::runtime_error("Could not load tilemap " + json_file + "!");
		return;
	}
	// Load the json data
	file >> mData;
	file.close();

	// Get the path to the map's folder.
	fs::path mapFolder(json_file);
	mapFolder = mapFolder.parent_path();

	mTiles	= mData["layers"][0]["data"].get<std::vector<int>>();
	mMapSize  = { mData["width"].get<int>(), mData["height"].get<int>() };
	mTexture  = resource->texture(mapFolder / mData["tilesets"][0]["image"].get<std::string>());
	mTileSize = { mData["tilewidth"].get<int>(), mData["tileheight"].get<int>() };

	loadVertices(autotile);
}

int Tilemap::getTile(int x, int y) const
{
	// Calculate the index.
	int index = x + y * mMapSize.x;
	/// If out of bounds, return 0.
	if (index < 0 || index >= mTiles.size())
		return 0;
	else
		return mTiles[index];   // Return the tile.
}

sf::Vector2i Tilemap::getMapSize() const
{
	return mMapSize;
}

sf::Vector2i Tilemap::getTileSize() const
{
	return mTileSize;
}

const nlohmann::json& Tilemap::getRawMapData() const
{
	return mData;
}

unsigned char Tilemap::getNeighborBitmask(int index)
{
	// Get x and y position.
	int x, y;
	x = index % mMapSize.x;
	y = index / mMapSize.x;

	// Array of neighboring positions.
	int neighbor_pos[4] = { x + (y - 1) * mMapSize.x,
							(x - 1) + y * mMapSize.x,
							(x + 1) + y * mMapSize.x,
							x + (y + 1) * mMapSize.x };

	// Get the four neighbors
	int neighbors[4] = { 0 };
	std::transform(neighbor_pos, neighbor_pos + 4, neighbors, [this](int pos) {
		if (pos < 0 || pos >= mTiles.size())
			return 1;
		else
			return int(mTiles[pos] != 0);
	});

	// Return the bitmasked sum
	unsigned char sum = (neighbors[0] << 0) |
						(neighbors[1] << 1) |
						(neighbors[2] << 2) |
						(neighbors[3] << 3);
	return sum;
}

void Tilemap::loadVertices(bool autotile)
{
	mVertices.clear();

	// Get the texture tilemap dimensions.
	sf::Vector2i tMapDim(mTexture->getSize().x / mTileSize.x,
						 mTexture->getSize().y / mTileSize.y);

	// Iter through all tiles
	int index = -1;   //< index++ is run first in the loop, so this is -1 to start.
	for (auto& tile_id : mTiles)
	{
		index++;

		// Skip if the tile_id is 0 (empty)
		if (tile_id == 0)
		{
			continue;
		}
		// Otherwise, get the texture id.
		int id = tile_id - 1;
		// Get the x and y position.
		int x = (index % mMapSize.x) * mTileSize.x;
		int y = (index / mMapSize.y) * mTileSize.y;

		// Get the tile's vertex positions on-screen.
		sf::Vector2i position[4] = {
			{ x, y },
			{ x + mTileSize.x, y },
			{ x + mTileSize.x, y + mTileSize.y },
			{ x, y + mTileSize.y }
		};

		// Get the texture index to use..
		int tex_index = autotile ? getNeighborBitmask(index) : id;
		// Get the texture x and y
		int tx, ty;
		tx = (tex_index % tMapDim.x) * mTileSize.x;
		ty = (tex_index / tMapDim.x) * mTileSize.y;

		// Get the texture coords.
		sf::Vector2i texture[4] = {
			{ tx, ty },
			{ tx + mTileSize.x, ty },
			{ tx + mTileSize.x, ty + mTileSize.y },
			{ tx, ty + mTileSize.y }
		};

		// Create the four vertices.
		mVertices.append(sf::Vertex(
			sf::Vector2f(position[0]),
			sf::Vector2f(texture[0])));
		mVertices.append(sf::Vertex(
			sf::Vector2f(position[1]),
			sf::Vector2f(texture[1])));
		mVertices.append(sf::Vertex(
			sf::Vector2f(position[2]),
			sf::Vector2f(texture[2])));
		mVertices.append(sf::Vertex(
			sf::Vector2f(position[3]),
			sf::Vector2f(texture[3])));
	}
}

void Tilemap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = mTexture;

	target.draw(mVertices, states);
}

}