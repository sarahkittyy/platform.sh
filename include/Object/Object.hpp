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

	/// Update priority. Lower values are higher priority. Default 10.
	unsigned int priority;

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

	/// Get the static level tilemap.
	const GFX::TiledTilemap& getStaticTilemap();

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

	/// Get the static level tilemap.
	std::function<const GFX::TiledTilemap&()> mGetStaticTilemap;

	// For initialization
	friend class Level::Level;
};

}