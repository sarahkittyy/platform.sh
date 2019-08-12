#pragma once

#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include "Editor/GUI/State/Machine.hpp"
#include "Editor/GUI/State/Objects.hpp"
#include "Editor/GUI/State/PropsLevel.hpp"
#include "Level/Level.hpp"
#include "State/Menu.hpp"
#include "State/State.hpp"
#include "imgui/imgui-SFML.h"
#include "imgui/imgui.h"
#include "nlohmann/json.hpp"
#include "pfd/portable-file-dialogs.h"

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
	/// Load the editor with an initial level.
	Edit(std::shared_ptr<Level::Level> level);
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
	/// Reset and deserialize the level from the given file.
	void loadFromFile(std::string file);
	/// Serialize the level to a file.
	void saveToFile(std::string file);

	/// Draws all static, base GUI.
	void drawBaseGUI();

	/// A panel, with a gui state machine and a visibility flag.
	struct _Panel
	{
		Editor::GUI::State::Machine machine;
		ImGuiWindowFlags flags;
		std::string name;
		bool visible;
	};
	// For convenience.
	typedef std::shared_ptr<_Panel> Panel;

	/// Create and initialize a panel.
	Panel createPanel(Editor::GUI::State::State* initialState,
					  std::string name,
					  ImGuiWindowFlags flags = 0);

	//! Panels
	Panel mProperties;
	Panel mObjects;

	/// Draw a panel to the screen.
	void drawPanel(Panel& panel);

	//! Camera panning.
	sf::Vector2f mMouseStart;
	sf::Vector2f mLevelStart;
	bool mPanning;
};

}