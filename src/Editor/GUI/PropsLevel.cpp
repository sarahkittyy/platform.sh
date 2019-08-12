#include "Editor/GUI/State/PropsLevel.hpp"

namespace Editor::GUI::State
{

PropsLevel::PropsLevel(Object::Props initialProps)
	: State(initialProps)
{
	std::fill(mLevelText, mLevelText + 100, '\0');
}

void PropsLevel::init()
{
}

void PropsLevel::draw()
{
	ImGui::Text("Level Text");
	if (ImGui::InputTextMultiline("###InputText", mLevelText, 100))
	{
		props().set({ { "levelText", std::string(mLevelText) } });
	}
}

std::string PropsLevel::title() const
{
	return "Properties";
}

}