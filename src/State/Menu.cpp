#include "State/Menu.hpp"

namespace State
{

void Menu::init()
{
	changeState(new Game());
}

}