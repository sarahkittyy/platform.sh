#pragma once

#include <SFML/Graphics.hpp>
#include "Audio/SoundEngine.hpp"
#include "GFX/AnimatedSprite.hpp"
#include "Object/Object.hpp"
#include "Object/Pushable.hpp"

namespace Object
{

/**
 * @brief Main player object, controllable with WASD.
 * 
 */
class Player : public Object, public Pushable
{
public:
	/**
	 * @brief Init the player
	 * 
	 * @param startPos The player's starting position, in tiles.
	 */
	Player(sf::Vector2f startPos);

	Player* create();
	Player* clone();

	/// Sets up the sprite and positioning.
	void init();

	void update();
	void updateTick();
	void reset();

	//* Methods for Pushable compatibility.
	void pushRight();   // Pushes the player right
	void pushLeft();	// Pushes the player left
	void pushUp();		// Pushes the player up
	void pushDown();	// Usually kills and resets the player.

	/// Resets the player. Use on death.
	void kill();

private:
	/// SFML draw() override.
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	/// The main player sprite.
	GFX::AnimatedSprite mPlayer;
	/// The player's sound engine.
	Audio::SoundEngine mSounds;

	/// The player's start position.
	sf::Vector2f mStartPos;
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

	/// The player death count.
	int mDeathCount;

	/**
	 * @brief For pushing the player.
	 * Since the player is updating last, the player is pushed before the previous
	 * position interpolation finishes.
	 * 
	 * This queued push is used at the next player update tick, to 
	 * properly append the motion.
	 * 
	 */
	sf::Vector2i mQueuedPush;

	/// For position interpolation.
	sf::Vector2i mInitialPosition;
	/// The next position to aim towards when interpolating for smooth animation
	sf::Vector2i mNextPosition;
};

}