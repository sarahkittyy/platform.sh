#include "Audio/SoundEngine.hpp"

namespace Audio
{

void SoundEngine::init(ResourceManager* resource)
{
	mResource = resource;
}

void SoundEngine::setSound(std::string name, std::string path)
{
	mSounds[name].setBuffer(*mResource->sound(path));
}

void SoundEngine::removeSound(std::string name)
{
	// Stop the sound just in case.
	mSounds[name].stop();
	mSounds.erase(name);
}

void SoundEngine::playSound(std::string name, float volume)
{
	// If the sound doesn't exist, throw.
	if (mSounds.find(name) == mSounds.end())
	{
		throw std::runtime_error("Could not find sound with name " + name + "\n");
	}

	mSounds[name].setVolume(volume);
	mSounds[name].play();
}

}