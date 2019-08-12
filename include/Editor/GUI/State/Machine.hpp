#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "Editor/GUI/State/State.hpp"
#include "ResourceManager.hpp"
#include "imgui/imgui-SFML.h"
#include "imgui/imgui.h"

namespace Editor::GUI::State
{

/**
 * @brief A gui state machine. Renders the gui specified by states, and allows for quick switching between states, for simpler gui tree development.
 * 
 */
class Machine
{
public:
	/**
	 * @brief Initialize the machine.
	 * 
	 * @param initialState The machine's initial state.
	 * @param window The app renderwindow.
	 * @param resource The app resourcemanager.
	 */
	void init(State* initialState, sf::RenderWindow* window, ResourceManager* resource);

	/// Set the state of the machine at the next available time to do so (after draw()).
	void setState(State* newState);

	/**
	 * @brief Get the title for the window from the active state.
	 * 
	 * @param id The constant identifier for the window, so the changing title names don't confuse ImGui.
	 * 
	 * @return std::string The intended window title. 
	 */
	std::string title(std::string id = "") const;

	/// Draw the active state to the current ImGui window.
	void draw();

private:
	/// The current active state.
	std::shared_ptr<State> mCurrentState;
	/// If not nullptr, this state will be set as active before the next draw() call.
	State* mNextState;

	/// Sets the state of the machine forcefully, without waiting for draw() to finish.
	void forceSetState(State* newState);

	/// The app window.
	sf::RenderWindow* mWindow;
	/// The app resource manager.
	ResourceManager* mResource;
};

}