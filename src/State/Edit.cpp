#include "State/Edit.hpp"

namespace State
{

Edit::Edit()
{
	mPropertiesPanelVisible = true;
}

Edit::~Edit()
{
	mBGMusic->stop();
}

void Edit::init()
{
	mPropertiesPanel.init(new Editor::GUI::State::Props(), &window(), &resource());

	mBGMusic = resource().music("assets/music/bg.flac");
	mBGMusic->setVolume(75);
	mBGMusic->setLoop(true);
	mBGMusic->play();
}

void Edit::update()
{
	// Draw GUI.
	drawPropertiesPanel();
	drawBaseGUI();


	// Draw sfml.
	window().clear(sf::Color::Black);


	// Render ImGui
	ImGui::SFML::Render(window());
	// Finish drawing.
	window().display();
}

void Edit::on(const sf::Event& event)
{
}

void Edit::drawBaseGUI()
{
	//! Main menu bar
	ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.3, 0.3, 0.6, 0.8));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.5, 0.5, 1, 1));
	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("Editor"))
	{
		if (ImGui::MenuItem("Exit"))
		{
			changeState(new Menu());
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("View"))
	{
		ImGui::MenuItem("Properties", nullptr, &mPropertiesPanelVisible);
		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
	ImGui::PopStyleColor(2);
}

void Edit::drawPropertiesPanel()
{
	if (mPropertiesPanelVisible)
	{
		ImGui::Begin(mPropertiesPanel.title("Properties").c_str(), &mPropertiesPanelVisible);
		mPropertiesPanel.draw();
		ImGui::End();
	}
}

}