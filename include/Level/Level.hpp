#pragma once

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "nlohmann/json.hpp"

#include "GFX/Tilemap.hpp"
#include "ResourceManager.hpp"

#include "Object/Object.hpp"
#include "Object/Objects.hpp"
#include "Object/Props.hpp"

namespace Level
{

/**
 * @brief Level class, loads from file, stores level information,
 * drawable and able to be started / stopped / reset.
 * 
 */
class Level : public sf::Drawable
{
public:
	/// For readability.
	typedef std::shared_ptr<Object::Object> ObjectPtr;

	/// Init
	Level();

	/// Init the level, and load the static tilemap.
	void init(sf::RenderWindow* window,
			  ResourceManager* resource);

	/// Serialize the level, level properties, and objects to JSON
	nlohmann::json serialize();
	/// Reset this level from ta.
	void deserialize(const nlohmann::json& data);

	/**
	 * @brief Link an object to the level. Also initializes the object.
	 * 
	 * @param object The object itself (new-allocated, i.e. new Object())
	 * 
	 * @returns A pointer to that object.
	 */
	template <typename Obj>
	Obj* addObject(Obj* object)
	{
		return dynamic_cast<Obj*>(addObjectGeneric(object));
	}

	/**
	 * @brief Add an object based on the name of the object, and the properties.
	 * 
	 * @tparam Obj The type of object to create.
	 * @param name The name of the object.
	 * @param props The object properties.
	 * @return Obj* The object itself.
	 */
	template <typename Obj>
	Obj* addObject(std::string name, Object::Props props)
	{
		return dynamic_cast<Obj*>(addObjectGeneric(name, props));
	}

	/**
	 * @brief Add an object to a level, without casting to any type.
	 * 
	 * @param object The object.
	 * @return Object::Object* A pointer to the newly created object. 
	 * 
	 * @see Level::addObject()
	 */
	Object::Object* addObjectGeneric(Object::Object* object);

	/**
	 * @brief Add the object to the level, without casting to the necessary type.
	 * 
	 * @param name The name of the object.
	 * @param props The object's properties.
	 * @return Object::Object* A pointer to that object.
	 */
	Object::Object* addObjectGeneric(std::string name, Object::Props props);

	/**
	 * @brief Delete the object from the level.
	 * 
	 * @param object The pointer to the object.
	 * @remarks The object will be nullified! Do not use the object after this call.
	 */
	void removeObject(Object::Object* object);

	/// Grab a list of all objects to which the given query function returns true.
	std::vector<ObjectPtr> queryObjects(std::function<bool(const Object::Props&)> query);

	/// Checks for collision with any object at a given grid point.
	bool isCollisionAt(sf::Vector2i pos);

	/// Set the position of the camera
	void setCameraPosition(sf::Vector2f pos);
	/// Get the position of the camera
	sf::Vector2f getCameraPosition();
	/// Set the size of the visible area.
	void setViewportSize(sf::Vector2f size);
	/// Get the level viewport size.
	sf::Vector2f getViewportSize();

	/// Set the game update tick speed.
	void setTickSpeed(sf::Time speed);
	/// Sets the level text font. Defaults to starmap.ttf
	void setFont(std::string path);
	/// Set the text to display on the top-right of the screen.
	void setDisplayText(std::string text);

	/// Start the level updating.
	void start();
	/// Reset all objects to default.
	void reset();
	/// Stop the level.
	void stop();
	/// Call once per frame -- updates the level.
	void update();

	/// Set the music stream to play in the background.
	void setMusic(std::string path);

	////////////////////////////
	//
	// Event methods
	//
	////////////////////////////

	/**
	 * @brief Emit an event
	 * 
	 * @param event The event name to emit.
	 * @param data The data tagged along with the event.
	 * 
	 * @remarks Can be called by objects.
	 */
	void emit(std::string event, nlohmann::json data);

	/**
	 * @brief Attach a handler to events, to be called when an event is emitted.
	 * 
	 * @param event The name of the event. 
	 * @param handler The event handler to attach.
	 * 
	 * @remarks If attached from an object, it can be fired during the update cycle
	 * of any other object -- so be careful with timings! Attach event data to a
	 * private object event queue if necessary, to handle in the object's own update() loop.
	 */
	void on(std::string event, std::function<void(const nlohmann::json&)> handler);

private:
	/// SFML draw() override.
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	/// App window.
	sf::RenderWindow* mWindow;
	/// App resource manager.
	ResourceManager* mResource;

	/// For naming objects and allowing them to be created via string identifiers.
	static std::vector<ObjectPtr> mObjectTemplates;
	/// Get the template object instance with the given name.
	static ObjectPtr getObject(std::string name);

	/// The text displayed at the top-right of the level.
	sf::Text mLevelText;
	/// The main font used for rendering level text.
	sf::Font* mFont;

	/// The level's background music.
	sf::Music* mBGMusic;

	/// The main level clock.
	sf::Clock mClock;
	/// The speed at which each game tick goes by.
	sf::Time mTickSpeed;
	/// Whether or not the level is updating.
	bool mRunning;

	/// The size of a map tile, for objects to snap to.
	static const sf::Vector2i TILESIZE;
	/// The size of the map's whole grid.
	static const sf::Vector2i GRIDSIZE;

	/// For convenience, see mEventCallbacks.
	typedef std::function<void(const nlohmann::json&)> EventCallback;
	/// A map of events to their list of attached event handler functions.
	std::unordered_map<std::string, std::vector<EventCallback>> mEventCallbacks;

	/// The camera viewport.
	sf::View mViewport;

	/// All level objects.
	std::vector<ObjectPtr> mObjects;
	/// All objects, sorted by priority.
	std::vector<ObjectPtr> mObjectsPriority;
	/// All objects, sorted by Z-index.
	std::vector<ObjectPtr> mObjectsZIndex;

	/// Sort the priority queue.
	void syncPriorityQueue();

	/// Sort the ZIndex queue.
	void syncZIndexQueue();
};

}