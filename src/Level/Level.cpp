#include "Level/Level.hpp"

namespace Level
{

std::unordered_map<std::string, Level::ObjectPtr>
	Level::mObjectTemplates({ { "Player", std::make_shared<Object::Player>() },
							  { "Tilemap", std::make_shared<Object::Tilemap>() },
							  { "ArrowPlatform", std::make_shared<Object::ArrowPlatform>() },
							  { "ArrowPlatformEnd", std::make_shared<Object::ArrowPlatformEnd>() } });

const sf::Vector2i Level::TILESIZE(32, 32);
const sf::Vector2i Level::GRIDSIZE(50, 50);

Level::Level()
	: mTickSpeed(sf::seconds(0.4f)),
	  mRunning(false)
{
	mClock.restart();
}

void Level::init(sf::RenderWindow* window,
				 ResourceManager* resource)
{
	// Save the resource manager.
	mResource = resource;
	mWindow   = window;

	/// Default text settings.
	mLevelText.setFillColor(sf::Color::White);
	mLevelText.setOutlineThickness(0);
	mLevelText.setCharacterSize(18);

	/// Load the default font.
	setFont("assets/fonts/starmap.ttf");
	/// Load the default music.
	setMusic("assets/music/bg.flac");

	mViewport.setSize(sf::Vector2f(mWindow->getSize()));
	mWindow->setView(mViewport);
	on("windowResized", [this](const nlohmann::json& data) {
		unsigned x, y;
		x = data.at("x").get<unsigned>();
		y = data.at("y").get<unsigned>();

		mViewport.setSize(x, y);
	});
}

nlohmann::json Level::serialize()
{
	nlohmann::json lvl;

	/// Base level properties.
	lvl["text"]		 = mLevelText.getString();
	lvl["tickSpeed"] = mTickSpeed.asSeconds();
}

void Level::deserialize(const nlohmann::json& data)
{
	mObjects.clear();
	mObjectsPriority.clear();
	mObjectsZIndex.clear();
}

Object::Object* Level::addObjectGeneric(Object::Object* object)
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
	object->mSetViewportSize   = [this](sf::Vector2f size) { setViewportSize(size); };
	object->mGetViewportSize   = [this]() { return getViewportSize(); };

	object->mSyncPriorityQueue = [this]() { syncPriorityQueue(); };
	object->mSyncZIndexQueue   = [this]() { syncZIndexQueue(); };

	object->mGridSize = &GRIDSIZE;
	object->mTileSize = &TILESIZE;

	object->mEmit	= [this](std::string event, nlohmann::json data) { emit(event, data); };
	object->mOnEvent = [this](std::string event, EventCallback callback) { on(event, callback); };

	object->mGetTickRate		 = [this]() { return mTickSpeed; };
	object->mGetCurrentClockTime = [this]() { return mClock.getElapsedTime(); };

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

Object::Object* Level::addObjectGeneric(std::string name, Object::Props props)
{
	return addObjectGeneric(mObjectTemplates.at(name)->create(props));
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
	mViewport.setCenter(pos.x, pos.y);
	mWindow->setView(mViewport);
}

sf::Vector2f Level::getCameraPosition()
{
	return mViewport.getCenter();
}

void Level::setViewportSize(sf::Vector2f size)
{
	mViewport.setSize(size);
	mWindow->setView(mViewport);
}

sf::Vector2f Level::getViewportSize()
{
	return mViewport.getSize();
}

void Level::setTickSpeed(sf::Time speed)
{
	mTickSpeed = speed;
}

void Level::setFont(std::string path)
{
	mFont = mResource->font(path);
	mLevelText.setFont(*mFont);
}

void Level::setDisplayText(std::string text)
{
	mLevelText.setString(text);

	// The padding to apply to the text from the top-right corner.
	const int PADDING = 10;
	// Update text positioning.
	mLevelText.setOrigin(mLevelText.getLocalBounds().width, 0);
	mLevelText.setPosition(mWindow->getSize().x - PADDING, PADDING);
}

void Level::start()
{
	mRunning = true;
	mBGMusic->play();
}

void Level::stop()
{
	mRunning = false;
	mBGMusic->pause();
}

void Level::reset()
{
	mClock.restart();
	mBGMusic->stop();
	mBGMusic->play();

	// Reset all objects.
	for (auto& object : mObjects)
	{
		object->reset();
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

void Level::setMusic(std::string path)
{
	// Stop the music if necessary.
	if (mBGMusic != nullptr)
	{
		mBGMusic->stop();
	}

	mBGMusic = mResource->music(path);
	mBGMusic->setVolume(75);
	mBGMusic->setLoop(true);
}

void Level::emit(std::string event, nlohmann::json data)
{
	// Call each handler for the given event.
	std::for_each(mEventCallbacks[event].begin(), mEventCallbacks[event].end(),
				  [&data](EventCallback& callback) {
					  callback(data);
				  });
}

void Level::on(std::string event, std::function<void(const nlohmann::json&)> handler)
{
	// Append the handler to the event callback map.
	mEventCallbacks[event].push_back(handler);
}

void Level::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	mWindow->setView(mWindow->getDefaultView());
	// Draw the text, untransformed in the top right.
	target.draw(mLevelText, states);
	mWindow->setView(mViewport);

	// Draw all objects
	for (auto& obj : mObjectsZIndex)
	{
		target.draw(*obj, states);
	}
}

}