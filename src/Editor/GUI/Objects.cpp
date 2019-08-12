#include "Editor/GUI/State/Objects.hpp"

namespace Editor::GUI::State
{

Objects::Objects(Object::Props props)
	: State(props)
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