#include "Editor/GUI/State/PropsLevel.hpp"

namespace Editor::GUI::State
{

PropsLevel::PropsLevel(Object::Props initialProps)
	: State(initialProps)
{
}

void PropsLevel::init()
{
}

void PropsLevel::draw()
{
	ImGui::Text("Properties");
}

std::string PropsLevel::title() const
{
	return "Properties";
}

}