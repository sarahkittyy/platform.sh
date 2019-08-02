#include "Object/ArrowPlatformEnd.hpp"

namespace Object
{

ArrowPlatformEnd::ArrowPlatformEnd(std::string turnTo)
	: mTurnTo(turnTo)
{
}

ArrowPlatformEnd::ArrowPlatformEnd(int degrees)
	: mTurnTo(""),
	  mDegrees(degrees)
{
}

void ArrowPlatformEnd::init()
{
	/// Configure this as an ArrowPlatformEnd object.
	props().set({ { "arrowPlatformEnd", true } });

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