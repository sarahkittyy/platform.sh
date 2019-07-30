#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include "GFX/TiledTilemap.hpp"
#include "ResourceManager.hpp"

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

protected:
	/// Returns a reference to the app resource manager.
	ResourceManager& resource();
	/// Add an object to the currently attached level.
	Object* addObject(Object* object);
	/// Remove an object from the level.
	void removeObject(Object* object);

	/// Set the position of the level camera
	void setCameraPosition(sf::Vector2f pos);
	/// Get the position of the level camera
	sf::Vector2f getCameraPosition();
	/// Set the scale of the level visible area.
	void setViewportScale(float scale);
	/// Get the level viewport scale.
	float getViewportScale();

	void setPriority(unsigned int priority);
	void setZIndex(unsigned int zindex);

	/// Get the static level tilemap.
	const GFX::TiledTilemap& staticTilemap();

	/// SFML draw() override.
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	/// Pointer to the app resource manager.
	ResourceManager* mResource;
	/// Function pointer to add an object to the currently linked level.
	std::function<Object*(Object*)> mAddObject;
	/// Initialized by Level::Level, removes & deallocates object from level.
	std::function<void(Object*)> mRemoveObject;
	/// Set the level camera position.
	std::function<void(sf::Vector2f)> mSetCameraPosition;
	/// Get the camera position.
	std::function<sf::Vector2f()> mGetCameraPosition;
	/// Set the level viewport scale
	std::function<void(float)> mSetViewportScale;
	/// Get the level viewport scale.
	std::function<float()> mGetViewportScale;
	/// Re-sync the level priority queue.
	std::function<void()> mUpdatePriorityQueue;
	/// Re-sync the level ZIndex queue.
	std::function<void()> mUpdateZIndexQueue;

	/// The update priority (lower -> first)
	unsigned int mPriority;
	/// The object Z index (lower -> drawn last)
	unsigned int mZIndex;

	/// The level static tilemap
	GFX::TiledTilemap* mStaticTilemap;

	// For initialization
	friend class Level::Level;
};

}