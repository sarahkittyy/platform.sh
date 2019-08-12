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
		// Save their icon textures.
		mObjects[templateObject->name()] = resource().texture(templateObject->icon());
	}
}

void Objects::draw()
{
	// Draw all object buttons.
	for (auto& [name, texture] : mObjects)
	{
		if (ImGui::ImageButton(*texture))
		{
		}
	}
}

std::string Objects::title() const
{
	return "Objects";
}

}