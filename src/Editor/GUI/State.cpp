#include "Editor/GUI/State/State.hpp"

namespace Editor::GUI::State
{

State::State()
{
}

State::~State()
{
}

void State::init()
{
}

void State::draw()
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
	mChangeState(newState);
}

}