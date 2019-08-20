#include "Level/Level.hpp"

namespace Level
{

std::vector<Level::ObjectPtr>
	Level::mObjectTemplates{
		std::make_shared<Object::Player>(),
		std::make_shared<Object::Tilemap>(),
		std::make_shared<Object::ArrowPlatform>(),
		std::make_shared<Object::ArrowPlatformEnd>()
	};

const sf::Vector2i Level::TILESIZE(32, 32);
const sf::Vector2i Level::GRIDSIZE(50, 50);

Level::Level()
	: mBGMusic(nullptr),
	  mTickSpeed(sf::seconds(0.4f)),
	  mRunning(false)
{
	mClock.restart();
	mViewport.setCenter(0.f, 0.f);
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
	mLevelText.setCharacterSize(20);

	/// Load the default font.
	setFont("assets/fonts/starmap.ttf");
	/// Load the default music.
	setMusic("assets/music/bg.flac");

	mViewport.setSize(sf::Vector2f(mWindow->getSize()));
	on("windowResized", [this](const nlohmann::json& data) {
		unsigned x, y;
		x = data.at("x").get<unsigned>();
		y = data.at("y").get<unsigned>();

		mViewport.setSize(x, y);
	});
}

nlohmann::json Level::serialize()
{
	using nlohmann::json;

	json lvl;

	/// Base level properties.
	lvl["text"]		 = mLevelText.getString();
	lvl["tickSpeed"] = mTickSpeed.asSeconds();
	lvl["objects"]   = json::array();
	json& objects	= lvl["objects"];

	for (auto& obj : mObjects)
	{
		std::string name  = obj->name();
		json initialProps = obj->initialProps();
		json activeProps  = obj->serialize();
		objects.push_back({ { "name", name },
							{ "initialProps", initialProps },
							{ "activeProps", activeProps } });
	}

	return lvl;
}

void Level::deserialize(const nlohmann::json& data)
{
	mObjects.clear();
	mObjectsPriority.clear();
	mObjectsZIndex.clear();

	// Load properties.
	mLevelText.setString(data["text"].get<std::string>());
	mTickSpeed = sf::seconds(data["tickSpeed"].get<float>());
	for (auto& obj : data["objects"].get<nlohmann::json::array_t>())
	{
		Object::Object* newObj = addObjectGeneric(obj["name"], Object::Props(obj["initialProps"]));
		newObj->deserialize(obj["activeProps"]);
	}
}

Object::Object* Level::addObjectGeneric(Object::Object* object)
{
	// Initialize the object.
	object->mResource = mResource;
	// Give the object a pointer to this level.
	object->mLevel = this;
	// Init the object
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
	return addObjectGeneric(getObject(name)->create(props));
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

std::vector<Level::ObjectPtr> Level::queryObjects(std::function<bool(const Object::Props&)> query) const
{
	std::vector<ObjectPtr> ret;

	for (auto& obj : mObjects)
		if (query(obj->getProps()))
			ret.push_back(obj);

	return ret;
}

const std::vector<Level::ObjectPtr>& Level::getObjectTemplates()
{
	return mObjectTemplates;
}

bool Level::isCollisionAt(sf::Vector2i pos) const
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
}

sf::Vector2f Level::getCameraPosition() const
{
	return mViewport.getCenter();
}

void Level::setViewportSize(sf::Vector2f size)
{
	mViewport.setSize(size);
}

sf::Vector2f Level::getViewportSize() const
{
	return mViewport.getSize();
}

const sf::View& Level::getViewport() const
{
	return mViewport;
}

void Level::setTickSpeed(sf::Time speed)
{
	mTickSpeed = speed;
}

sf::Time Level::getTickSpeed() const
{
	return mTickSpeed;
}

sf::Time Level::getCurrentClockTime() const
{
	return mClock.getElapsedTime();
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

std::string Level::getDisplayText() const
{
	return mLevelText.getString();
}

sf::Vector2i Level::tileSize()
{
	return TILESIZE;
}

sf::Vector2i Level::gridSize()
{
	return GRIDSIZE;
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
	if (mBGMusic)
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
	// Draw the text, untransformed in the top right.
	target.draw(mLevelText, states);

	// Transform by the viewport.
	mWindow->setView(mViewport);

	// Draw all objects
	for (auto& obj : mObjectsZIndex)
	{
		target.draw(*obj, states);
	}

	// Undo the viewport transform
	mWindow->setView(mWindow->getDefaultView());
}

Level::ObjectPtr Level::getObject(std::string name)
{
	for (auto& obj : mObjectTemplates)
	{
		if (obj->name() == name)
		{
			return obj;
		}
	}
	return nullptr;
}

}