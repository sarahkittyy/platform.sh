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
		// Store the icon with the template object
		Objects::ObjectButton out{
			.sample  = templateObject,
			.texture = resource().texture(templateObject->icon())
		};
		// Save the bundled data.
		mObjects.push_back(out);
	}
}

void Objects::draw()
{
	// Draw all object buttons.
	for (auto& button : mObjects)
	{
		if (ImGui::ImageButton(*button.texture, sf::Vector2f(32, 32)))
		{
			//TODO: Begin placing the object.
		}
		// Hover tooltip
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("%s\n> %s",
							  button.sample->name().c_str(),
							  button.sample->desc().c_str());
		}
		ImGui::SameLine();
	}
}

std::string Objects::title() const
{
	return "Objects";
}

}