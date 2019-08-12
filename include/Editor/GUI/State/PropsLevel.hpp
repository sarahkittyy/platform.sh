#pragma once

#include "Editor/GUI/State/State.hpp"

namespace Editor::GUI::State
{

/**
 * @brief The base gui properties panel.
 * 
 */
class PropsLevel : public State
{
public:
	PropsLevel(Object::Props initialProps = Object::Props());

	void init();
	void draw();

	std::string title() const;
};

}