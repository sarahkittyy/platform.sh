#pragma once

#include <SFML/Graphics.hpp>
#include <stdexcept>
#include <string>
#include "GFX/AnimatedSprite.hpp"
#include "Object/Object.hpp"
#include "Object/Props.hpp"
#include "Object/Pushable.hpp"
#include "ResourceManager.hpp"
#include "nlohmann/json.hpp"

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

	/// Bare constructor, do not call init(). Used for create() and clone() only.
	ArrowPlatform();

	/**
	 * @brief 
	 * 
	 * @remarks Properties
	 * 	- pos: Vector2 => Initial platform position
	 * 	- dir: int => Initial arrow direction (See `Direction`) 
	 */
	ArrowPlatform(Props props);

	ArrowPlatform* create(Props props);
	ArrowPlatform* clone();

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
	void updateEndpoints(sf::Vector2f currentPosition);
	/// Returns the current dir rotated `degrees` degrees, floored to the nearest 90 degree mark.
	Direction turnDegrees(int degrees);

	/**
	 * @brief Push all pushable objects in the level.
	 * 
	 * @param currentPosition The position before any movement.
	 * @param offset The final offset to apply to the object.
	 */
	void pushPushables(sf::Vector2f currentPosition, sf::Vector2f offset);

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