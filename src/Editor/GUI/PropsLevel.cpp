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
	/// Level text setting.
	ImGui::Text("Level Text");
	ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput |
								ImGuiInputTextFlags_NoHorizontalScroll |
								ImGuiInputTextFlags_EnterReturnsTrue |
								ImGuiInputTextFlags_CtrlEnterForNewLine;
	if (ImGui::InputTextMultiline("###InputText", mLevelText, 100, ImVec2(150, 50), flags))
	{
		props().set({ { "levelText", std::string(mLevelText) } });
	}

	/// Tickrate setting.
	ImGui::Text("Tickrate (sec.)");
	if (ImGui::SliderFloat("", &mTickrate, 0.1f, 2.f, "%.1fs"))
	{
		props().set({ { "tickrate", mTickrate } });
	}
}

std::string PropsLevel::title() const
{
	return "Properties";
}

}