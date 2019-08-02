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

sf::SoundBuffer* ResourceManager::sound(std::string path)
{
	if (mSounds.find(path) == mSounds.end())
	{
		mSounds[path].loadFromFile(path);
	}

	return &mSounds[path];
}

sf::Music* ResourceManager::music(std::string path)
{
	if (mMusic.find(path) == mMusic.end())
	{
		mMusic[path].openFromFile(path);
	}

	return &mMusic[path];
}