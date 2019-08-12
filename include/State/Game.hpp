#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "Level/Factory/Levels.hpp"
#include "Level/Level.hpp"
#include "Object/Object.hpp"
#include "ResourceManager.hpp"
#include "State/Edit.hpp"
#include "State/State.hpp"
#include "imgui/imgui-SFML.h"
#include "imgui/imgui.h"

namespace State
{

/**
 * @brief Main game state.
 * 
 */
class Game : public State
{
public:
	/// Init the game, and use the test level.
	Game();
	/// Init the game with a pre-generated level.
	Game(std::shared_ptr<Level::Level> level, bool fromEditor = false);
	/// Stops the level.
	~Game();

	void init();
	void update();

	void on(const sf::Event& event);

private:
	/// The level the game plays.
	std::shared_ptr<Level::Level> mLevel;
	/// If we just came from the editor. Enables extra GUI and escaping back to the editor.
	bool mFromEditor;
	/// Entry point for editor-related functionality.
	void fromEditor();
};

}