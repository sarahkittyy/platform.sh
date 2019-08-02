#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include "GFX/TiledTilemap.hpp"
#include "Object/Props.hpp"
#include "ResourceManager.hpp"
#include "nlohmann/json.hpp"

// So that Object::Object can friend Level and allow for initialization.
namespace Level
{
class Level;
}

namespace Object
{

/**
 * @brief Base game object class. Contains methods for initialization, 
 * drawing, updating, and resetting.
 * 
 */
class Object : public sf::Drawable
{
public:
	/// Sets the default priority.
	Object();
	/// For inheritance
	virtual ~Object();
	/// Called once all protected resources are loaded (ResourceManager, etc)
	virtual void init();
	/// Called once per frame
	virtual void update();
	/// Called once per game tick.
	virtual void updateTick();
	/// Should reset the object back to it's initial state.
	virtual void reset();

	/// Get the object's update priority.
	unsigned int getPriority();
	/// Get the object's z index.
	unsigned int getZIndex();

	/// Derivable, used by other objects to determine collision. Implement for solid objects.
	virtual bool isSolidAt(sf::Vector2i pos);

	/// Returns a constant reference to the object's properties.
	const Props& getProps();

protected:
	/// Returns a reference to the app resource manager.
	ResourceManager& resource();
	/// Get the object's properties -- editable for derived classes.
	Props& props();

	/// Add an object to the currently attached level.
	Object* addObject(Object* object);
	/// Remove an object from the level.
	void removeObject(Object* object);
	/// Query the parent level for objects of which the query function returns true.
	std::vector<std::shared_ptr<Object>> queryObjects(std::function<bool(const Props&)> query);

	/// Check for any collision with any object in the level at a given point.
	bool isCollisionAt(sf::Vector2i pos);

	/// Set the position of the level camera
	void setCameraPosition(sf::Vector2f pos);
	/// Get the position of the level camera
	sf::Vector2f getCameraPosition();
	/// Set the scale of the level visible area.
	void setViewportScale(float scale);
	/// Get the level viewport scale.
	float getViewportScale();

	/// Set the object's update priority. (Lower = updated first)
	void setPriority(unsigned int priority);
	/// Set the object's z index. (Higher = further back)
	void setZIndex(unsigned int zindex);

	/// Convert coords from real coords to grid coords (not rounded)
	sf::Vector2f getGridPosition(sf::Vector2f actualPos);
	/// Convert tile coordinates to actual coordinates (not rounded)
	sf::Vector2f getActualPosition(sf::Vector2f tilePos);

	/// Get the size of the grid.
	const sf::Vector2i& gridSize();
	/// Get the size of a single grid tile.
	const sf::Vector2i& tileSize();

	/// Returns a value 0<=t<=1 representing the fraction of time passed until the next tick.
	float interpolationFactor() const;

	/// Emit an event to the level.
	void emit(std::string event, nlohmann::json data);
	// Attach a callback to be run on level event emitted.
	void on(std::string event, std::function<void(const nlohmann::json& data)> callback);

	/// SFML draw() override.
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	/// The update priority (lower -> first)
	unsigned int mPriority;
	/// The object Z index (lower -> drawn last)
	unsigned int mZIndex;

	/// The object's properties.
	Props mProps;

	////////////////////////////
	//
	// All variables past this point initialized by Level::addObject()
	//
	////////////////////////////

	/// Pointer to the app resource manager.
	ResourceManager* mResource;

	/// Function pointer to add an object to the currently linked level.
	std::function<Object*(Object*)> mAddObject;
	/// Initialized by Level::Level, removes & deallocates object from level.
	std::function<void(Object*)> mRemoveObject;
	/// Query the parent level for other objects matching a query function.
	std::function<std::vector<std::shared_ptr<Object>>(std::function<bool(const Props&)>)> mQueryObjects;

	/// Check for any collision with any object in the level at a given point.
	std::function<bool(sf::Vector2i)> mIsCollisionAt;

	/// Set the level camera position.
	std::function<void(sf::Vector2f)> mSetCameraPosition;
	/// Get the camera position.
	std::function<sf::Vector2f()> mGetCameraPosition;
	/// Set the level viewport scale
	std::function<void(float)> mSetViewportScale;
	/// Get the level viewport scale.
	std::function<float()> mGetViewportScale;
	/// Re-sync the level priority queue.
	std::function<void()> mSyncPriorityQueue;
	/// Re-sync the level ZIndex queue.
	std::function<void()> mSyncZIndexQueue;

	/// Get the current time of the restarting clock that manages tickrate.
	std::function<sf::Time()> mGetCurrentClockTime;
	/// Get the level's tickrate.
	std::function<sf::Time()> mGetTickRate;

	/// Emit an event to the level.
	std::function<void(std::string, nlohmann::json)> mEmit;
	/// Attach an event callback to
	std::function<void(std::string, std::function<void(const nlohmann::json&)>)> mOnEvent;

	/// The map grid size
	sf::Vector2i const* mGridSize;
	/// The map tile size.
	sf::Vector2i const* mTileSize;

	// For initialization
	friend class Level::Level;
};

}