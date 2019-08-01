#pragma once

#include <string>

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

	/// Update properties using a JSON object.
	void set(const nlohmann::json::value_type& data);

	/// Test if the value at the given key matches the given value.
	bool test(const nlohmann::json::json_pointer& key,
			  const nlohmann::json::value_type& value) const;

	/// Returns a constant reference to all properties.
	const nlohmann::json& get() const;

private:
	/// The actual held properties.
	nlohmann::json mProps;
};

}