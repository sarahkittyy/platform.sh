#include "State/Machine.hpp"

namespace State
{

Machine::Machine(State* initial,
				 sf::RenderWindow* window,
				 ResourceManager* resource)
	: mWindow(window),
	  mResource(resource),
	  mQueuedState(nullptr)
{
	// Activate the current state
	resetCurrentState(initial);
}

void Machine::on(const sf::Event& event)
{
	// Pass on the event to the current state
	mCurrentState->on(event);
}

void Machine::update()
{
	// Update the current state.
	mCurrentState->update();
	// If there's a queue'd state change...
	if (mQueuedState != nullptr)
	{
		// Reset the state.
		resetCurrentState(mQueuedState);
		mQueuedState = nullptr;
	}
}

void Machine::changeState(State* newstate)
{
	// If it's not nullptr, throw an error, because the state can't change itself twice.
	if (mQueuedState != nullptr)
	{
		throw std::runtime_error("Attempting to change state more than once / update.");
	}
	// Queue the state change.
	mQueuedState = newstate;
}

void Machine::resetCurrentState(State* state)
{
	mCurrentState.reset(state);
	// Set state env vars
	mCurrentState->mWindow = mWindow;
	mCurrentState->mResource = mResource;
	mCurrentState->mChangeState =
		std::bind(&Machine::changeState, this, std::placeholders::_1);
	// Call init()
	mCurrentState->init();
}

}