#pragma once

#include <SFML/Graphics.hpp>
#include "GFX/AnimatedSprite.hpp"
#include "Object/Object.hpp"

namespace Object
{

/**
 * @brief Main player object, controllable with WASD.
 * 
 */
class Player : public Object
{
public:
	/**
	 * @brief Init the player
	 * 
	 * @param startPos The player's starting position, in tiles.
	 */
	Player(sf::Vector2i startPos);
	/// Sets up the sprite and positioning.
	void init();

	void update();
	void updateTick();

	void reset();

private:
	/// SFML draw() override.
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	/// The main player sprite.
	GFX::AnimatedSprite mPlayer;

	/// The player's start position.
	sf::Vector2i mStartPos;

	/// Set the position of the player, snapped to the grid.
	void setPosition(sf::Vector2i pos);
	/// Get the position of the player, aligned to the grid.
	sf::Vector2i getPosition();

	////////////////////////////
	//
	// Physics methods
	//
	////////////////////////////

	void moveRight();
	void moveLeft();
	bool jump();

	bool mJustJumped = false;

	void postMove();
};

}