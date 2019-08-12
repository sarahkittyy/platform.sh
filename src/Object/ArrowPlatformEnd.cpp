#include "Object/ArrowPlatformEnd.hpp"

namespace Object
{

ArrowPlatformEnd::ArrowPlatformEnd(Props props)
	: Object(props),
	  mPosition(Props::toVector<float>(props.get("/pos"_json_pointer)))
{
	if (props.exists("/turnTo"_json_pointer))
	{
		mTurnTo = props.get("/turnTo"_json_pointer);
	}
	if (props.exists("/degrees"_json_pointer))
	{
		mDegrees = props.get("/degrees"_json_pointer);
	}
}

ArrowPlatformEnd* ArrowPlatformEnd::create(Props props)
{
	return new ArrowPlatformEnd(props);
}

ArrowPlatformEnd* ArrowPlatformEnd::clone()
{
	return new ArrowPlatformEnd(initialProps());
}

const std::string ArrowPlatformEnd::name() const
{
	return "ArrowPlatformEnd";
}

const std::string ArrowPlatformEnd::icon() const
{
	return "assets/icons/arrowplatformend.png";
}

/// Object is constant after initialization.
nlohmann::json ArrowPlatformEnd::serialize() const
{
	return nlohmann::json();
}

void ArrowPlatformEnd::deserialize(const nlohmann::json& data)
{
}
//////////////////

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