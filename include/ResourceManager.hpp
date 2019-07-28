#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>

/**
 * @brief Manages expensive resources to ensure only one of each
 * is loaded into memory at a time.
 * 
 */
class ResourceManager
{
public:
	/**
	 * @brief Retrieve a texture at the given path.
	 * 
	 * @param path The path to the texture image.
	 * @return sf::Texture* A pointer to the sfml texture.
	 */
	sf::Texture* texture(std::string path);

private:
	/// Map of paths to their textures.
	std::unordered_map<std::string, sf::Texture> mTextures;
};