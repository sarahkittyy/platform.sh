#pragma once

#include <SFML/Audio.hpp>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include "ResourceManager.hpp"

namespace Audio
{

/**
 * @brief Stores and names sounds for easy playability. Instance once per object
 * that plays sounds.
 * 
 */
class SoundEngine
{
public:
	/**
	 * @brief Init the sound engine
	 * 
	 * @param resource The app resource manager.
	 */
	void init(ResourceManager* resource);

	/**
	 * @brief Add a named sound.
	 * 
	 * @param name The name of the sound.
	 * @param path The path to the sound file.
	 */
	void setSound(std::string name, std::string path);

	/**
	 * @brief Remove a sound.
	 * 
	 * @param name The name of the sound.
	 */
	void removeSound(std::string name);

	/**
	 * @brief Play a sound.
	 * 
	 * @param name The name of the sound.
	 * @param volume The volume to play the sound at.
	 */
	void playSound(std::string name, float volume = 100);

private:
	/// The app resource manager.
	ResourceManager* mResource;

	/// Sounds mapped to their names.
	std::unordered_map<std::string, sf::Sound> mSounds;
};

}