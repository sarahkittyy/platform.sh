#include "Editor/GUI/State/Objects.hpp"

namespace Editor::GUI::State
{

Objects::Objects(Level::Level* level)
	: mLevel(level)
{
}

void Objects::init()
{
	// Iterate over all template objects
	for (auto& templateObject : Level::Level::getObjectTemplates())
	{
	}
}

void Objects::draw()
{
}

std::string Objects::title() const
{
	return "Objects";
}

}