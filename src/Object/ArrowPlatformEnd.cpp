#include "Object/ArrowPlatformEnd.hpp"

namespace Object
{

ArrowPlatformEnd::ArrowPlatformEnd(sf::Vector2f pos, std::string turnTo)
	: mPosition(pos),
	  mTurnTo(turnTo)
{
}

ArrowPlatformEnd::ArrowPlatformEnd(sf::Vector2f pos, int degrees)
	: mPosition(pos),
	  mTurnTo(""),
	  mDegrees(degrees)
{
}

ArrowPlatformEnd* ArrowPlatformEnd::create()
{
}

ArrowPlatformEnd* ArrowPlatformEnd::clone()
{
}

void ArrowPlatformEnd::init()
{
	/// Configure this as an ArrowPlatformEnd object.
	props().set({ { "arrowPlatformEnd", true } });
	/// Add a property for x/y position.
	props().set({ { "position", std::vector<float>{ mPosition.x, mPosition.y } } });

	/// Configure platform behavior properties.
	if (mTurnTo.empty())
	{
		props().set({ { "rotateDegrees", mDegrees } });
	}
	else
	{
		props().set({ { "turnTo", mTurnTo } });
	}
}

}