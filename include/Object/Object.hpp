#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>
#include "GFX/Tilemap.hpp"
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
class Object : public sf::Drawable, public sf::Transformable
{
public:
	/**
	 * @brief Init the object.
	 * 
	 * @param props The object's initial properties.
	 */
	Object(Props props = {});
	/// For inheritance
	virtual ~Object();

	/// Create an object of this type.
	virtual Object* create(Props props) = 0;
	/// Clone this object.
	virtual Object* clone() = 0;

	/// The object's name.
	virtual const std::string name() const;

	//* Editor vars
	/// Path to an icon image representing this object.
	virtual const std::string icon() const;
	/// A description of this object.
	virtual const std::string desc() const;

	/// Serialize the object properties to json
	virtual nlohmann::json serialize() const = 0;
	/// Load the object's properties from json data.
	virtual void deserialize(const nlohmann::json& data) = 0;

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

	/// Get the intiial object properties.
	const Props& initialProps();

	/// Get the parent level
	Level::Level& level() const;

	/// Returns a value 0<=t<=1 representing the fraction of time passed until the next tick.
	float interpolationFactor() const;

	/// Set the object's update priority. (Lower = updated first)
	void setPriority(unsigned int priority);
	/// Set the object's z index. (Higher = further back)
	void setZIndex(unsigned int zindex);

	/// Convert coords from real coords to grid coords (not rounded)
	sf::Vector2f getGridPosition(sf::Vector2f actualPos);
	/// Convert tile coordinates to actual coordinates (not rounded)
	sf::Vector2f getActualPosition(sf::Vector2f tilePos);

	/// SFML draw() override.
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	/// The update priority (lower -> first)
	unsigned int mPriority;
	/// The object Z index (lower -> drawn last)
	unsigned int mZIndex;

	/// The parent level.
	Level::Level* mLevel;

	/// The object's initial properties.
	Props mInitialProps;
	/// The object's active properties.
	Props mProps;

	////////////////////////////
	//
	// All variables past this point initialized by Level::addObject()
	//
	////////////////////////////

	/// Pointer to the app resource manager.
	ResourceManager* mResource;

	// For initialization
	friend class Level::Level;
};

}