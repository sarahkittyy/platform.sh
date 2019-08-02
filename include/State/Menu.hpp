#pragma once

#include "State/Game.hpp"
#include "State/State.hpp"

namespace State
{

/**
 * @brief The main menu. This is the first state the app loads into.
 * 
 */
class Menu : public State
{
public:
	void init();
};

}