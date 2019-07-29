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

	mObjects.push_back(ObjectPtr(object));
	/// Get the newly pushed object shared_ptr.
	ObjectPtr* obj_ptr = &mObjects.back();
	/// Insert it into the priority queues.
	insertIntoZQueue(obj_ptr);
	insertIntoPriorityQueue(obj_ptr);


	return mObjects.back().get();
}

void Level::insertIntoZQueue(ObjectPtr* ptr)
{
	// Find the first element greater than the z-index.
	auto found = std::find_if(mObjectsZIndex.begin(), mObjectsZIndex.end(),
							  [ptr](auto& obj) {
								  return (*obj)->zindex > (*ptr)->zindex;
							  });
	if (found == mObjectsZIndex.end())
	{
		mObjectsZIndex.push_back(ptr);
	}
	else
	{
		// Insert the object into the found position.
		mObjectsZIndex.insert(found, ptr);
	}
}

void Level::insertIntoPriorityQueue(ObjectPtr* ptr)
{
	// Find the first element greater than this object's.
	auto found = std::find_if(mObjectsPriority.begin(), mObjectsPriority.end(),
							  [ptr](auto& obj) {
								  return (*obj)->priority > (*ptr)->priority;
							  });
	if (found == mObjectsPriority.end())
	{
		mObjectsPriority.push_back(ptr);
	}
	else
	{
		// Insert the object into the found position.
		mObjectsPriority.insert(found, ptr);
	}
}

void Level::removeObject(Object::Object* object)
{
	std::remove_if(mObjects.begin(), mObjects.end(),
				   [object](std::shared_ptr<Object::Object>& obj) {
					   return obj.get() == object;
				   });

	// Remove the object from the priority queues as well.
	std::remove_if(mObjectsPriority.begin(), mObjectsPriority.end(),
				   [object](auto& objptr) {
					   return objptr->get() == object;
				   });
	std::remove_if(mObjectsZIndex.begin(), mObjectsZIndex.end(),
				   [object](auto& objptr) {
					   return objptr->get() == object;
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

	/// Game tick updates.
	if (mClock.getElapsedTime() >= mTickSpeed)
	{
		mClock.restart();

		/// Frame update and tick update all objects.
		for (auto& obj : mObjectsPriority)
		{
			(*obj)->update();
			(*obj)->updateTick();
		}
	}
	else   // If it's not time to tick updte, just frame update.
	{
		for (auto& obj : mObjectsPriority)
		{
			(*obj)->update();
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

	// Draw all objects
	for (auto& obj : mObjectsZIndex)
	{
		target.draw(**obj, states);
	}
}
}