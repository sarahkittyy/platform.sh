#include "Editor/GUI/State/State.hpp"

namespace Editor::GUI::State
{

State::State(Object::Props initialProps)
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

std::string State::title() const
{
	return "Untitled";
}

const Object::Props& State::getProps()
{
	return mProps;
}

Object::Props& State::props()
{
	return mProps;
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