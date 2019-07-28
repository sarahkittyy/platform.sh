#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include "ResourceManager.hpp"

namespace State
{

// So the state machine can be friended to allow for initialization.
class Machine;

/**
 * @brief A single state the game can be in.
 * 
 */
class State
{
public:
	/// Virtual destructor for inheritance.
	virtual ~State();
	/// Called once all protected environment variables are set (window, resourcemanager)
	virtual void init();
	/// Called for each event sent by SFML.
	virtual void on(const sf::Event& event);
	/// Called once per frame
	virtual void update();

protected:
	/// Retrieves the app window.
	sf::RenderWindow& window();
	/// Retrieves the app resource manager.
	ResourceManager& resource();
	/// Change the app state.
	void changeState(State* newState);

private:
	/// Pointer to the main app window.
	sf::RenderWindow* mWindow;
	/// Pointer to the app resource manager.
	ResourceManager* mResource;
	/// Function pointer to change the app state.
	std::function<void(State*)> mChangeState;

	friend class Machine;
};

}