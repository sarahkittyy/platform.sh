#include "State/State.hpp"

namespace State
{

State::~State()
{
}

void State::init()
{
}

void State::on(const sf::Event& event)
{
}

void State::update()
{
}

sf::RenderWindow& State::window()
{
	return *mWindow;
}

ResourceManager& State::resource()
{
	return *mResource;
}

void State::changeState(State* newState)
{
	// Just call the function pointer to change the state.
	mChangeState(newState);
}

}