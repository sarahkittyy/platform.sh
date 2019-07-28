#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <memory>
#include <stdexcept>
#include "ResourceManager.hpp"
#include "State/State.hpp"

namespace State
{

class Machine
{
public:
	/**
	 * @brief Init the machine
	 * 
	 * @param initial The initial machine state.
	 * @param window Pointer to the SFML window.
	 * @param resource Pointer to the app resource manager.
	 */
	Machine(State* initial,
			sf::RenderWindow* window,
			ResourceManager* resource);

	/// Called on every SFML event, passes events onto the active state.
	void on(const sf::Event& event);
	/// Called every frame, updates the active state.
	void update();
	/**
	 * @brief Safely changes the machine state.
	 * @remarks Call with a new-allocated state, i.e. changeState(new State()).
	 * 
	 * @param newState The new state to switch to.
	 */
	void changeState(State* newState);

private:
	/// The SFML window
	sf::RenderWindow* mWindow;
	/// Main resource manager.
	ResourceManager* mResource;
	/// The current state.
	std::shared_ptr<State> mCurrentState;
	/// The queue'd state, to be activated safely after mCurrentState->update()
	State* mQueuedState;

	/// Force resets the current state, as well as initializing state member vars.
	void resetCurrentState(State* state);
};

}