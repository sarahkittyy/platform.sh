#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include "Object/Props.hpp"
#include "ResourceManager.hpp"
#include "imgui/imgui-SFML.h"
#include "imgui/imgui.h"

namespace Editor::GUI::State
{

// For friending with state.
class Machine;

/**
 * @brief A state that an imgui window can be in. Managed by the GUI state machine.
 * 
 */
class State
{
public:
	/**
	 * @brief Construct the state. 
	 * 
	 * @remarks window() and resource() are not initialized in the constructor,
	 * so be careful.
	 * 
	 */
	State(Object::Props initialProps = Object::Props());
	virtual ~State();

	/// Initialize all variables
	virtual void init();
	/// Draw to the active
	virtual void draw();

	/// The state's window title.
	virtual std::string title() const;

	/// For external objects to access the properties.
	const Object::Props& getProps();

protected:
	/// Grab the app render window.
	sf::RenderWindow& window();
	/// Grab the app resource manager.
	ResourceManager& resource();
	/// For derived classes to access the properties.
	Object::Props& props();

	/// Change the state machine's state.
	void changeState(State* newState);

private:
	/// So machine can initialize private members before calling init().
	friend class Machine;

	/// The state's properties.
	Object::Props mProps;

	/// The app window
	sf::RenderWindow* mWindow;
	/// The app resource manager.
	ResourceManager* mResource;

	/// Init'd by Machine, changes the state of the gui machine.
	std::function<void(State* state)> mChangeState;
};

}