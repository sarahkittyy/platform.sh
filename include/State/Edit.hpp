#pragma once

#include <SFML/Graphics.hpp>
#include "Editor/GUI/State/Machine.hpp"
#include "Editor/GUI/State/Props.hpp"
#include "Level/Level.hpp"
#include "State/Menu.hpp"
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
	/// Sets some primitive defaults.
	Edit();
	/// Stops playing music.
	~Edit();

	void init();
	void update();

	void on(const sf::Event& event);

private:
	/// Editor background music.
	sf::Music* mBGMusic;

	/// The main level used to edit.
	std::shared_ptr<Level::Level> mLevel;
	//! Level-related functions
	/// Reset the level to empty.
	void newLevel();

	/// Draws all static, base GUI.
	void drawBaseGUI();

	/// The left-hand properties panel.
	void drawPropertiesPanel();
	Editor::GUI::State::Machine mPropertiesPanel;
	bool mPropertiesPanelVisible;
};

}