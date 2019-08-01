#include "Level/Level.hpp"

namespace Level
{

Level::Level()
	: mTickSpeed(sf::seconds(0.5f)),
	  mRunning(false),
	  TILESIZE(32, 32),
	  GRIDSIZE(50, 50),
	  mCameraPosition(0, 0),
	  mViewportScale(1)
{
	mClock.restart();
}

void Level::init(sf::RenderWindow* window,
				 ResourceManager* resource)
{
	// Save the resource manager.
	mResource = resource;
	mWindow   = window;

	updateCameraTransform();
}

Object::Object* Level::addObject(Object::Object* object)
{
	// Initialize the object.
	object->mResource = mResource;

	object->mAddObject	= [this](Object::Object* obj) { return addObject(obj); };
	object->mRemoveObject = [this](Object::Object* obj) { removeObject(obj); };
	object->mQueryObjects = [this](std::function<bool(const Object::Props&)> query) { return queryObjects(query); };

	object->mIsCollisionAt = [this](sf::Vector2i pos) { return isCollisionAt(pos); };

	// Bind camera controls.
	object->mSetCameraPosition = [this](sf::Vector2f pos) { setCameraPosition(pos); };
	object->mGetCameraPosition = [this]() { return getCameraPosition(); };
	object->mSetViewportScale  = [this](float scale) { setViewportScale(scale); };
	object->mGetViewportScale  = [this]() { return getViewportScale(); };

	object->mSyncPriorityQueue = [this]() { syncPriorityQueue(); };
	object->mSyncZIndexQueue   = [this]() { syncZIndexQueue(); };

	object->mGridSize = &GRIDSIZE;
	object->mTileSize = &TILESIZE;

	object->init();

	// Store the object in a smart pointer.
	ObjectPtr obj(object);

	mObjects.push_back(obj);
	mObjectsPriority.push_back(obj);
	mObjectsZIndex.push_back(obj);

	syncPriorityQueue();
	syncZIndexQueue();

	return mObjects.back().get();
}

void Level::removeObject(Object::Object* object)
{
	std::remove_if(mObjects.begin(), mObjects.end(),
				   [object](std::shared_ptr<Object::Object>& obj) {
					   return obj.get() == object;
				   });

	// Remove the object from the priority queues as well.
	std::remove_if(mObjectsPriority.begin(), mObjectsPriority.end(),
				   [object](auto& obj) {
					   return obj.get() == object;
				   });
	std::remove_if(mObjectsZIndex.begin(), mObjectsZIndex.end(),
				   [object](auto& obj) {
					   return obj.get() == object;
				   });
}

std::vector<Level::ObjectPtr> Level::queryObjects(std::function<bool(const Object::Props&)> query)
{
	std::vector<ObjectPtr> ret;

	for (auto& obj : mObjects)
		if (query(obj->getProps()))
			ret.push_back(obj);

	return ret;
}

bool Level::isCollisionAt(sf::Vector2i pos)
{
	// Check all objects with the "collideable" prop.
	auto collideableObjects = queryObjects([](auto& props) {
		return props.test("/collideable"_json_pointer, true);
	});

	// Return true if any of the objects are collideable at that point.
	return std::any_of(collideableObjects.begin(), collideableObjects.end(),
					   [&pos](auto& object) {
						   return object->isSolidAt(pos);
					   });
}

void Level::syncPriorityQueue()
{
	/// The lower the priority, the quicker it's updated.
	std::sort(mObjectsPriority.begin(), mObjectsPriority.end(),
			  [](auto& obj1, auto& obj2) {
				  return obj1->getPriority() < obj2->getPriority();
			  });
}

void Level::syncZIndexQueue()
{
	/// The higher the z index, the quicker it's updated.
	std::sort(mObjectsZIndex.begin(), mObjectsZIndex.end(),
			  [](auto& obj1, auto& obj2) {
				  return obj1->getZIndex() < obj2->getZIndex();
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
			obj->update();
			obj->updateTick();
		}
	}
	else   // If it's not time to tick updte, just frame update.
	{
		for (auto& obj : mObjectsPriority)
		{
			obj->update();
		}
	}
}

void Level::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= mTransform;

	// Draw all objects
	for (auto& obj : mObjectsZIndex)
	{
		target.draw(*obj, states);
	}
}

}