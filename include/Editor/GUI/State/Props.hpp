#pragma once

#include "Editor/GUI/State/State.hpp"

namespace Editor::GUI::State
{

/**
 * @brief The base gui properties panel.
 * 
 */
class Props : public State
{
public:
	void init();
	void draw();

	std::string title() const;

private:
};

}