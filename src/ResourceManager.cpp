#include "ResourceManager.hpp"

sf::Texture* ResourceManager::texture(std::string path)
{
	// Create the texture if it doesn't exist.
	if (mTextures.find(path) == mTextures.end())
	{
		mTextures[path].loadFromFile(path);
	}

	return &mTextures[path];
}

sf::Font* ResourceManager::font(std::string path)
{
	if (mFonts.find(path) == mFonts.end())
	{
		mFonts[path].loadFromFile(path);
	}

	return &mFonts[path];
}