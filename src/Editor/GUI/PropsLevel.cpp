#include "Editor/GUI/State/PropsLevel.hpp"

namespace Editor::GUI::State
{

PropsLevel::PropsLevel(Level::Level* level)
	: mLevel(level)
{
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

	std::string levelText = mLevel->getDisplayText();
	levelText.reserve(100);
	if (ImGui::InputTextMultiline("###InputText", levelText.data(), 100, ImVec2(150, 50), flags))
	{
		mLevel->setDisplayText(levelText);
	}

	/// Tickrate setting.
	ImGui::Text("Tickrate (sec.)");
	mTickrate = mLevel->getTickSpeed().asSeconds();
	if (ImGui::SliderFloat("", &mTickrate, 0.1f, 2.f, "%.1fs"))
	{
		mLevel->setTickSpeed(sf::seconds(mTickrate));
	}
}

std::string PropsLevel::title() const
{
	return "Properties";
}

}