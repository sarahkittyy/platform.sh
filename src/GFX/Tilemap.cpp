#include "GFX/Tilemap.hpp"

namespace GFX
{

Tilemap::Tilemap()
{
	mVertices.setPrimitiveType(sf::Quads);
}

void Tilemap::init(ResourceManager* resource)
{
	mResource = resource;
}

void Tilemap::loadFromTiled(std::string json_file)
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
	mTexture  = mResource->texture(mapFolder / mData["tilesets"][0]["image"].get<std::string>());
	mTileSize = { mData["tilewidth"].get<int>(), mData["tileheight"].get<int>() };

	reloadVertices();
}

void Tilemap::load(sf::Vector2i mapSize,
				   sf::Vector2i tileSize,
				   std::string image)
{
	mTiles.resize(mapSize.x * mapSize.y);
	mMapSize  = mapSize;
	mTileSize = tileSize;
	mTexture  = mResource->texture(image);

	reloadVertices();
}

void Tilemap::autotile()
{
	reloadVertices(true);
}

void Tilemap::setTile(int x, int y, int id)
{
	int pos = x + y * mMapSize.x;
	if (pos < mTiles.size())
	{
		mTiles[pos] = id;
		// Update the vertices too.
		setQuad(pos, id);
	}
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

void Tilemap::reloadVertices(bool autotile)
{
	mVertices.clear();
	mVertices.resize(mMapSize.x * mMapSize.y * 4);

	// Get the texture tilemap dimensions.
	mTextureMapSize = sf::Vector2i(mTexture->getSize().x / mTileSize.x,
								   mTexture->getSize().y / mTileSize.y);

	// Iter through all tiles
	int index = -1;   //< index++ is run first in the loop, so this is -1 to start.
	for (auto& tile_id : mTiles)
	{
		index++;

		setQuad(index, tile_id, autotile);
	}
}

void Tilemap::setQuad(int pos, int id, bool autotile)
{
	// Move the position to vertexarray position
	int arr_pos = pos * 4;

	// Clear vertices for air.
	if (id == 0)
	{
		for (int i = 0; i < 4; ++i)
		{
			mVertices[arr_pos + i] = sf::Vertex(sf::Vector2f(0, 0), sf::Vector2f(0, 0));
		}
		return;
	}
	// Otherwise, convert to real texture position.
	id--;

	// Get the x and y position.
	int x = (pos % mMapSize.x) * mTileSize.x;
	int y = (pos / mMapSize.y) * mTileSize.y;

	// Get the tile's vertex positions on-screen.
	sf::Vector2i position[4] = {
		{ x, y },
		{ x + mTileSize.x, y },
		{ x + mTileSize.x, y + mTileSize.y },
		{ x, y + mTileSize.y }
	};

	// Get the texture index to use..
	int tex_index = autotile ? getNeighborBitmask(pos) : id;
	// Get the texture x and y
	int tx, ty;
	tx = (tex_index % mTextureMapSize.x) * mTileSize.x;
	ty = (tex_index / mTextureMapSize.x) * mTileSize.y;

	// Get the texture coords.
	sf::Vector2i texture[4] = {
		{ tx, ty },
		{ tx + mTileSize.x, ty },
		{ tx + mTileSize.x, ty + mTileSize.y },
		{ tx, ty + mTileSize.y }
	};

	// Set the four vertices.
	mVertices[arr_pos + 0] = (sf::Vertex(
		sf::Vector2f(position[0]),
		sf::Vector2f(texture[0])));
	mVertices[arr_pos + 1] = (sf::Vertex(
		sf::Vector2f(position[1]),
		sf::Vector2f(texture[1])));
	mVertices[arr_pos + 2] = (sf::Vertex(
		sf::Vector2f(position[2]),
		sf::Vector2f(texture[2])));
	mVertices[arr_pos + 3] = (sf::Vertex(
		sf::Vector2f(position[3]),
		sf::Vector2f(texture[3])));
}

void Tilemap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = mTexture;

	target.draw(mVertices, states);
}

}