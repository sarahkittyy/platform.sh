#pragma once

#include <SFML/Graphics.hpp>
#include "GFX/TiledTilemap.hpp"
#include "ResourceManager.hpp"

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
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	/// The main level clock.
	sf::Clock mClock;
	/// The speed at which each game tick goes by.
	sf::Time mTickSpeed;
	/// Whether or not the level is updating.
	bool mRunning;

	/// The static tiles that make up the map.
	GFX::TiledTilemap mStaticMap;
}

}