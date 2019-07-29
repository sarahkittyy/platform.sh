#pragma once

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <functional>
#include <string>
#include <utility>
#include <vector>

#include "GFX/TiledTilemap.hpp"
#include "ResourceManager.hpp"

#include "Object/Object.hpp"

namespace Level
{

/**
 * @brief Level class, loads from file, stores level information,
 * drawable and able to be started / stopped / reset.
 * 
 */
class Level : public sf::Drawable, public sf::Transformable
{
public:
	Level();

	/// Load the static tilemap.
	void init(ResourceManager* resource,
			  std::string file,
			  bool autotile = false);

	/**
	 * @brief Link an object to the level. Also initializes the object.
	 * 
	 * @param object The object itself (new-allocated, i.e. new Object())
	 * 
	 * @returns A pointer to that object.
	 */
	Object::Object* addObject(Object::Object* object);

	/**
	 * @brief Delete the object from the level.
	 * 
	 * @param object The pointer to the object.
	 * @remarks The object will be nullified! Do not use the object after this call.
	 */
	void removeObject(Object::Object* object);

	/// Set the game update tick speed.
	void setTickSpeed(sf::Time speed);
	/// Start the level updating.
	void start();
	/// Reset all objects to default.
	void reset();
	/// Stop the level.
	void stop();
	/// Call once per frame -- updates the level.
	void update();

private:
	/// SFML draw() override.
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	/// App resource manager.
	ResourceManager* mResource;

	/// The main level clock.
	sf::Clock mClock;
	/// The speed at which each game tick goes by.
	sf::Time mTickSpeed;
	/// Whether or not the level is updating.
	bool mRunning;

	/// The static tiles that make up the map.
	GFX::TiledTilemap mStaticMap;
	/// All level objects.
	std::vector<std::shared_ptr<Object::Object>> mObjects;
};

}