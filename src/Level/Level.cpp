#include "Level/Level.hpp"

namespace Level
{

Level::Level()
	: mRunning(false)
{
	mClock.restart();
}

void Level::init(ResourceManager* resource,
				 std::string file,
				 bool autotile = false)
{
	// Load the static tilemap.
	mStaticMap.load(resource, file, autotile);
}

void Level::setTickSpeed(sf::Time speed)
{
	mTickSpeed = speed;
}

void Level::start()
{
	mRunning = true;
}

void Level::stop()
{
	mRunning = false;
}

void Level::reset()
{
	mClock.restart();
}

void Level::update()
{
	if (!mRunning) return;

	/// Game tick updates.
	if (mClock.getElapsedTime() >= mTickSpeed)
	{
		mClock.restart();
	}
}

void Level::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(mStaticMap);
}

}