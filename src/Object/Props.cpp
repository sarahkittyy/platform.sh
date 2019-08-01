#include "Object/Props.hpp"

namespace Object
{

Props::Props()
{
}

void Props::set(const nlohmann::json::value_type& data)
{
	// Update properties.
	mProps.update(data);
}

bool Props::test(const nlohmann::json::json_pointer& key,
				 const nlohmann::json::value_type& value) const
{
	try
	{
		return mProps[key] == value;
	}
	catch (...)   //? Maybe make this catch more robust?
	{
		return false;
	}
}

const nlohmann::json& Props::get() const
{
	return mProps;
}

}