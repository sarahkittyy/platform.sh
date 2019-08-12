#pragma once

#include <SFML/Graphics.hpp>
#include "Editor/GUI/State/Machine.hpp"
#include "Editor/GUI/State/Props.hpp"
#include "State/State.hpp"
#include "imgui/imgui-SFML.h"
#include "imgui/imgui.h"

namespace State
{

/**
 * @brief The main editor state.
 * 
 */
class Edit : public State
{
public:
	void init();
	void update();

	void on(const sf::Event& event);

private:
	/// Editor background music.
	sf::Music* mBGMusic;

	/// The left-hand properties panel.
	void drawPropertiesPanel();
	/// The state machine for the properties panel.
	Editor::GUI::State::Machine mPropertiesPanel;
};

}