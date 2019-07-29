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
	 * @param startPos The player's starting position.
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
};

}