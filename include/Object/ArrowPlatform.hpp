#pragma once

#include <SFML/Graphics.hpp>
#include <stdexcept>
#include <string>
#include "GFX/AnimatedSprite.hpp"
#include "Object/Object.hpp"
#include "Object/Props.hpp"
#include "ResourceManager.hpp"

namespace Object
{

/**
 * @brief Moving arrow platform. Moves 1 block / tick, turned by ArrowPlatformEnd instances.
 * 
 * @remarks Will phase through solid collision, only turning on ArrowPlatformEnd instances.
 * 
 */
class ArrowPlatform : public Object
{
public:
	/// All possible directions the arrow platform can travel in a tick.
	enum Direction
	{
		LEFT,
		UP,
		RIGHT,
		DOWN
	};   //* Conveniently ordered to cycle clockwise. (see turnDegrees)

	/// Init the platform, with the initial position and travel direction.
	ArrowPlatform(sf::Vector2f pos, Direction dir = RIGHT);

	/// Sprite initialization.
	void init();

	/// Interpolation and animation updates.
	void update();

	/// Position and collision checks.
	void updateTick();

	/// For other objects to collide with this object.
	bool isSolidAt(sf::Vector2i pos);

	/// Resets the platform to it's initial direction and position.
	void reset();

private:
	/// Draw the platform.
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	/// The graphical platform component.
	GFX::AnimatedSprite mPlatform;

	/// Save the initial position and direction for reset()
	sf::Vector2f mInitialPosition;
	Direction mInitialDirection;

	/// The position of the platform, irrelevant of ongoing interpolation.
	sf::Vector2f mInitialTickPosition;
	/// The (pixel-based) coordinate of the intended next platform position, to be interpolated in update().
	sf::Vector2f mIntendedNextPosition;

	/// Find neighboring arrowplatform instances, and act accordingly.
	void updateEndpoints(sf::Vector2f& currentPosition);
	/// Returns the current dir rotated `degrees` degrees, floored to the nearest 90 degree mark.
	Direction turnDegrees(int degrees);

	/// The direction the platform is travelling in.
	Direction mDir;
	/// Just converts mDir to a string, for animation purposes.
	std::string getDirectionAnimation();
	/// Gets the intended direction to move based on mDir.
	sf::Vector2f getDirectionOffset();
	/// Convert a "left", "right", etc. string to a Direction. Inverse of getDirectionAnimation
	Direction getDirection(std::string dirStr);
};

}