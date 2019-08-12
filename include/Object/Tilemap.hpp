#pragma once

#include <SFML/Graphics.hpp>
#include "GFX/Tilemap.hpp"
#include "Object/Object.hpp"
#include "ResourceManager.hpp"

namespace Object
{

/**
 * @brief Object wrapper for a static tilemap.
 * 
 */
class Tilemap : public Object
{
public:
	/**
	 * @brief Initialize the map.
	 * 
	 * @remarks Properties
	 * 	- mapFile: string => Path to the map json file.
	 * 	- autotile: boolean => Whether or not to autotile the map.
	 */
	Tilemap(Props props = Props());

	Tilemap* create(Props props);
	Tilemap* clone();

	/// Returns the internal map.
	GFX::Tilemap& map();

	const std::string name() const;
	const std::string icon() const;
	nlohmann::json serialize() const;
	void deserialize(const nlohmann::json& data);

	/// Initializes the tilemap graphic
	void init();

	/// Returns true for non-zero tiles at the given position.
	bool isSolidAt(sf::Vector2i pos);

private:
	/// SFML draw() override.
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	/// The main tilemap graphical component.
	GFX::Tilemap mMap;
};

}