#include "Editor/GUI/State/Machine.hpp"

namespace Editor::GUI::State
{

Machine::Machine(State* initialState, sf::RenderWindow* window, ResourceManager* resource)
	: mNextState(nullptr),
	  mWindow(window),
	  mResource(resource)
{
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