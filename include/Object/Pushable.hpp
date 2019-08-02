#pragma once

#include <SFML/Graphics.hpp>

namespace Object
{

/**
 * @brief Inherited class to define a pushable object.
 * Can be used with the "pushable" property to
 * allow an object to be customly pushed by other objects at will.
 * 
 */
class Pushable
{
public:
	/// For inheritance
	virtual ~Pushable();

	virtual void pushRight() = 0;
	virtual void pushLeft()  = 0;
	virtual void pushUp()	= 0;
	virtual void pushDown()  = 0;

	/// Used so that objects pushing you know where you are.
	virtual sf::Vector2i getPushablePosition() = 0;
};

}