#include "Editor/GUI/State/Objects.hpp"

namespace Editor::GUI::State
{

Objects::Objects(Object::Props initialProps)
	: State(initialProps)
{
}

void Objects::init()
{
}

void Objects::draw()
{
}

std::string Objects::title() const
{
	return "Objects";
}

}