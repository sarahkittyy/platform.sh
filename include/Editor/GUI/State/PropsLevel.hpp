#pragma once

#include "Editor/GUI/State/State.hpp"
#include "Level/Level.hpp"

namespace Editor::GUI::State
{

/**
 * @brief The base gui properties panel, for editing level properties.
 * 
 */
class PropsLevel : public State
{
public:
	PropsLevel(Level::Level* level);

	void init();
	void draw();

	std::string title() const;

private:
	/// The level we are modifying.
	Level::Level* mLevel;

	//* Storage
	float mTickrate;   /// Level tickrate.

	//* ------
};

}