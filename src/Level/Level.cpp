#include "Level/Level.hpp"

namespace Level
{

Level::Level()
	: mRunning(false),
	  mCameraPosition(0, 0),
	  mViewportScale(1)
{
	mClock.restart();
}

void Level::init(sf::RenderWindow* window,
				 ResourceManager* resource,
				 std::string file,
				 bool autotile)
{
	// Save the resource manager.
	mResource = resource;
	mWindow   = window;
	// Load the static tilemap.
	mStaticMap.load(resource, file, autotile);
	updateCameraTransform();
}

Object::Object* Level::addObject(Object::Object* object)
{
	// Initialize the object.
	object->mResource = mResource;
	object->mAddObject =
		std::bind(&Level::addObject, this, std::placeholders::_1);
	object->mRemoveObject =
		std::bind(&Level::removeObject, this, std::placeholders::_1);
	// Bind camera controls.
	object->mSetCameraPosition =
		std::bind(&Level::setCameraPosition, this, std::placeholders::_1);
	object->mGetCameraPosition =
		std::bind(&Level::getCameraPosition, this);
	object->mSetViewportScale =
		std::bind(&Level::setViewportScale, this, std::placeholders::_1);
	object->mGetViewportScale =
		std::bind(&Level::getViewportScale, this);

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

void Level::setCameraPosition(sf::Vector2f pos)
{
	mCameraPosition = -1.f * pos;
	updateCameraTransform();
}

sf::Vector2f Level::getCameraPosition()
{
	return mCameraPosition;
}

void Level::setViewportScale(float scale)
{
	mViewportScale = scale;
	updateCameraTransform();
}

float Level::getViewportScale()
{
	return mViewportScale;
}

void Level::updateCameraTransform()
{
	// Refresh the transform
	mTransform = sf::Transform::Identity;

	// Calculate the actual camera position (top-left)
	sf::Vector2f actualCam(
		mCameraPosition.x + mWindow->getSize().x / 2.f,
		mCameraPosition.y + mWindow->getSize().y / 2.f);

	// Move the transform to the camera position.
	mTransform.translate(actualCam);

	// Scale the viewport.
	mTransform.scale(mViewportScale, mViewportScale, mCameraPosition.x, mCameraPosition.y);
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

	// Sort all objects in order of priority (lowest to highest).
	std::sort(mObjects.begin(), mObjects.end(),
			  [](auto& obj1, auto& obj2) {
				  return obj1->priority < obj2->priority;
			  });

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

const GFX::TiledTilemap& Level::getTiledTilemap()
{
	return mStaticMap;
}

void Level::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= mTransform;
	target.draw(mStaticMap, states);
}

}