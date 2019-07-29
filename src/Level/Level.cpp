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
	// Save the resource manager.
	mResource = resource;
	// Load the static tilemap.
	mStaticMap.load(resource, file, autotile);
}

Object::Object* Level::addObject(Object::Object* object)
{
	// Initialize the object.
	object->mResource = mResource;
	object->mAddObject =
		std::bind(&Level::addObject, this, std::placeholders::_1);
	object->mRemoveObject =
		std::bind(&Level::removeObject, this, std::placeholders::_1);
	object->init();

	mObjects.push_back(std::shared_ptr<Object::Object>(object));

	return mObjects.back().get();
}

void Level::removeObject(Object::Object* object)
{
	std::remove_if(mObjects.begin(), mObjects.end(),
				   [object](std::shared_ptr<Object::Object>& obj) {
					   return obj.get() == object;
				   });
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

	// Reset all objects.
	for (auto& object : mObjects)
	{
		object.reset();
	}
}

void Level::update()
{
	if (!mRunning) return;

	// Note: strange update order used to optimize
	// when it's time for both a frame and a tick update,
	// so as to not run two loops.

	/// Game tick updates.
	if (mClock.getElapsedTime() >= mTickSpeed)
	{
		mClock.restart();

		/// Frame update and tick update all objects.
		for (auto& obj : mObjects)
		{
			obj->update();
			obj->updateTick();
		}
	}
	else   // If it's not time to tick updte, just frame update.
	{
		for (auto& obj : mObjects)
		{
			obj->update();
		}
	}
}

void Level::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(mStaticMap);
}

}