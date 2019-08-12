#pragma once

#include "Editor/GUI/State/State.hpp"

namespace Editor::GUI::State
{

class Objects : public State
{
public:
	Objects(Object::Props initialProps = Object::Props());

	void init();
	void draw();

	std::string title() const;
};

}