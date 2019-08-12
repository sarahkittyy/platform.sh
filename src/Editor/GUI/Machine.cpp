#include "Editor/GUI/State/Machine.hpp"

namespace Editor::GUI::State
{

void Machine::init(State* initialState, sf::RenderWindow* window, ResourceManager* resource)
{
	mNextState = nullptr;
	mWindow	= window;
	mResource  = resource;
	forceSetState(initialState);
}

void Machine::setState(State* newState)
{
	// If setState() is called with new-allocated states
	// more than once, free up the old state allocated.
	if (mNextState != nullptr)
		delete mNextState;
	// Queue the state change.
	mNextState = newState;
}

std::string Machine::title(std::string id) const
{
	return mCurrentState->title() + "###" + id;
}

void Machine::draw()
{
	// Draw the current state.
	mCurrentState->draw();

	// If there's a new queue'd state, use it.
	if (mNextState != nullptr)
	{
		forceSetState(mNextState);
		mNextState = nullptr;
	}
}

const Object::Props& Machine::getProps()
{
	return mCurrentState->getProps();
}

void Machine::forceSetState(State* newState)
{
	/// Init state vars.
	newState->mChangeState = std::bind(&Machine::setState, this, std::placeholders::_1);
	newState->mWindow	  = mWindow;
	newState->mResource	= mResource;
	/// Set the active state, and init.
	mCurrentState.reset(newState);
	mCurrentState->init();
}

}