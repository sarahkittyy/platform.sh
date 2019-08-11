#pragma once

#include <stdexcept>
#include <string>

#include <SFML/System.hpp>
#include "nlohmann/json.hpp"

namespace Object
{

/**
 * @brief A properties container for all objects. Properties can be set by the parent object,
 * and queried by other objects or by the level to match specifid object traits.
 * 
 */
class Props
{
public:
	/// Init
	Props();
	/// Init with a default value.
	Props(const nlohmann::json::value_type& initial);

	/// Cast to json
	operator nlohmann::json() const;

	/// Convert a sf::Vector2 to a json object with properties *x* and *y*
	template <typename T>
	static nlohmann::json fromVector(sf::Vector2<T> vec)
	{
		return { { "x", vec.x }, { "y", vec.y } };
	}

	template <typename T>
	static nlohmann::json fromVector(T x, T y)
	{
		return { { "x", x }, { "y", y } };
	}

	/// Convert a json objects with props "x" and "y" to a Vector2
	template <typename T>
	static sf::Vector2<T> toVector(const nlohmann::json& vec)
	{
		return sf::Vector2<T>(vec["x"], vec["y"]);
	}

	/// Update properties using a JSON object.
	Props& set(const nlohmann::json::value_type& data);

	/// Checks if a given key exists at all.
	bool exists(const nlohmann::json::json_pointer& key) const;

	/// Test if the value at the given key matches the given value.
	bool test(const nlohmann::json::json_pointer& key,
			  const nlohmann::json::value_type& value) const;

	/// Returns a constant reference to all properties.
	const nlohmann::json& get() const;
	/// Get the json value at the given key.
	const nlohmann::json::value_type& get(const nlohmann::json::json_pointer& key) const;

private:
	/// The actual held properties.
	nlohmann::json mProps;

	/*
	 * Common properties:
	 * 
	 * collideable: true if the object has any collision at all. Used by Level::isCollisionAt()
	 * pushable: true if the object inherits Object::Pushable and is, well, pushable.
	 */
};

}