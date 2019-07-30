#include "Level/Level.hpp"

namespace Level
{

Level::Level()
	: mTickSpeed(sf::seconds(0.5f)),
	  mRunning(false),
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
	object->mResource	 = mResource;
	object->mAddObject	= [this](Object::Object* obj) { return addObject(obj); };
	object->mRemoveObject = [this](Object::Object* obj) { removeObject(obj); };
	// Bind camera controls.
	object->mSetCameraPosition = [this](sf::Vector2f pos) { setCameraPosition(pos); };
	object->mGetCameraPosition = [this]() { return getCameraPosition(); };
	object->mSetViewportScale  = [this](float scale) { setViewportScale(scale); };
	object->mGetViewportScale  = [this]() { return getViewportScale(); };

	object->mStaticTilemap = &mStaticMap;

	object->mUpdatePriorityQueue = [this]() { syncPriorityQueue(); };
	object->mUpdateZIndexQueue   = [this]() { syncZIndexQueue(); };

	object->init();

	mObjects.push_back(ObjectPtr(object));
	/// Get the newly pushed object shared_ptr.
	ObjectPtr* obj_ptr = &mObjects.back();
	mObjectsPriority.push_back(obj_ptr);
	mObjectsZIndex.push_back(obj_ptr);

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
				   [object](auto& objptr) {
					   return objptr->get() == object;
				   });
	std::remove_if(mObjectsZIndex.begin(), mObjectsZIndex.end(),
				   [object](auto& objptr) {
					   return objptr->get() == object;
				   });
}

void Level::syncPriorityQueue()
{
	/// The lower the priority, the quicker it's updated.
	std::sort(mObjectsPriority.begin(), mObjectsPriority.end(),
			  [](auto& obj1, auto& obj2) {
				  return (*obj1)->getPriority() < (*obj2)->getPriority();
			  });
}

void Level::syncZIndexQueue()
{
	/// The higher the z index, the quicker it's updated.
	std::sort(mObjectsZIndex.begin(), mObjectsZIndex.end(),
			  [](auto& obj1, auto& obj2) {
				  return (*obj1)->getZIndex() < (*obj2)->getZIndex();
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