#include "Editor/GUI/State/PropsLevel.hpp"

namespace Editor::GUI::State
{

PropsLevel::PropsLevel(Level::Level* level)
	: mLevel(level)
{
	std::fill(mLevelText, mLevelText + 100, (char)0);
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
	std::fill(mLevelText, mLevelText + 100, (char)0);
	std::strcpy(mLevelText, levelText.c_str());
	if (ImGui::InputTextMultiline("###InputText",
								  mLevelText, 100,
								  ImVec2(150, 50), flags))
	{
		mLevel->setDisplayText(mLevelText);
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