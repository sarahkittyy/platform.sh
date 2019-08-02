#pragma once

#include "Object/Object.hpp"

namespace Object
{

/**
 * @brief Marks a stopping point for an arrow platform. Defines a rotation modifier, so
 * all arrow platforms colliding with this object will turn / rotate in the direction
 * defined in the properties.
 * 
 * @remarks Rotation properties.
 * - turnTo = "left", "right", "up", "down" // Checked first, will always make the platform face
 * this way
 * - rotateDegrees (int) // Checked if turnTo doesn't exist. Rotates n degrees clockwise.
 * 
 */
class ArrowPlatformEnd : public Object
{
public:
	/// Init the end with "left", "right", etc, as a property.
	ArrowPlatformEnd(sf::Vector2f pos, std::string turnTo);
	/// Init the platform so that all interacting platforms turn `degrees` degrees clockwise.
	ArrowPlatformEnd(sf::Vector2f pos, int degrees);

	/// Initialize the props for use.
	void init();

private:
	/// The position to place the platform.
	sf::Vector2f mPosition;

	/// If defined, used to set turnTo prop.
	std::string mTurnTo;
	/// If mTurnTo empty, sets rotateDegrees prop.
	int mDegrees;
};

}