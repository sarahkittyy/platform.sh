#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
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

protected:
	/// Returns a reference to the app resource manager.
	ResourceManager& resource();
	/// Add an object to the currently attached level.
	Object* addObject(Object* object);
	/// Remove an object from the level.
	void removeObject(Object* object);

	/// SFML draw() override.
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	/// Pointer to the app resource manager.
	ResourceManager* mResource;
	/// Function pointer to add an object to the currently linked level.
	std::function<Object*(Object*)> mAddObject;
	/// Initialized by Level::Level, removes & deallocates object from level.
	std::function<void(Object*)> mRemoveObject;

	// For initialization
	friend class Level::Level;
};

}