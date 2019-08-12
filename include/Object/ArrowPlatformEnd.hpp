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
	/**
	 * @brief Init the arrow platform end
	 * 
	 * @remarks Properties
	 * 	- pos: Vector2 => The platform position.
	 * 	- [degrees: int] => The degrees to rotate the platform by.
	 * 	- [turnTo: string] => If no degrees specified, the direction to turn the platform to.
	 */
	ArrowPlatformEnd(Props props =
						 Props()
							 .set({ { "pos", Props::fromVector(0.f, 0.f) } }));

	/// Initialize the props for use.
	void init();

	ArrowPlatformEnd* create(Props props);
	ArrowPlatformEnd* clone();

	const std::string name() const;
	const std::string icon() const;
	nlohmann::json serialize() const;
	void deserialize(const nlohmann::json& data);

private:
	/// The position to place the platform.
	sf::Vector2f mPosition;

	/// If defined, used to set turnTo prop.
	std::string mTurnTo;
	/// If mTurnTo empty, sets rotateDegrees prop.
	int mDegrees;
};

}