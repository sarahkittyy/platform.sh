#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
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
	Player(sf::Vector2f startPos);
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
	sf::Vector2f mStartPos;

	/// Set the position of the player, snapped to the grid.
	void setPosition(sf::Vector2f pos);
	/// Get the position of the player, aligned to the grid.
	sf::Vector2f getPosition();
	/// Set the position to a tile on the grid, except interpolated across the tickrate.
	void setPositionInterpolated(sf::Vector2i pos);
	/// Move the interpolated next position by offset.
	void moveInterpolated(sf::Vector2i offset);
	/// Get the intended next position when the movement is being interpolated.
	sf::Vector2i getPositionInterpolated();

	////////////////////////////
	//
	// Physics methods
	//
	////////////////////////////

	/// True if the player is airborne.
	bool airborne();

	/// Moves the player one right, if possible
	void moveRight();
	/// Moves the player one left, if possible
	void moveLeft();
	/// Attempts to jump
	bool jump();
	/// Drops the player down one.
	void fall();

	/// For position interpolation.
	sf::Vector2i mInitialPosition;
	/// The next position to aim towards when interpolating for smooth animation
	sf::Vector2i mNextPosition;
};

}