#pragma once

#include <SFML/Graphics.hpp>
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
};

}