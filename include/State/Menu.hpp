#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include "GFX/TiledTilemap.hpp"
#include "State/Game.hpp"
#include "State/State.hpp"
#include "Util/ImGuiShell.hpp"
#include "imgui/imgui-SFML.h"
#include "imgui/imgui.h"

namespace State
{

/**
 * @brief The main menu. This is the first state the app loads into.
 * 
 */
class Menu : public State
{
public:
	Menu();

	void init();
	void update();

private:
	/// Sets up all menu gui onto the screen. Run before rendering, each frame.
	void drawGUI();
	/// Update all title animations.
	void updateAnimations();

	/// Starts the game (also stopping all music).
	void startGame();

	/// For animation timing.
	sf::Clock mClock;
	/// The menu background music.
	sf::Music* mBGMusic;

	/// The title.
	sf::Text mTitle;
	/// The input shell.
	Util::ImGuiShell mShell;

	/// The background demo map to run.
	GFX::TiledTilemap mDemoMap;
};

}