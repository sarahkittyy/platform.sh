#include "Editor/GUI/State/Props.hpp"

namespace Editor::GUI::State
{

void Props::init()
{
}

void Props::draw()
{
	ImGui::Text("Properties");
}

std::string Props::title() const
{
	return "Properties";
}

}