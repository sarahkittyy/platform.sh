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

	/// Reset the level.
	newLevel();
}

void Edit::update()
{
	// Draw GUI.
	drawPropertiesPanel();
	drawBaseGUI();


	// Draw sfml.
	window().clear(sf::Color::Black);

	// Draw the level in the back.
	window().draw(*mLevel);

	// Render ImGui
	ImGui::SFML::Render(window());
	// Finish drawing.
	window().display();
}

void Edit::on(const sf::Event& event)
{
}

void Edit::newLevel()
{
	if (mLevel)
	{
		mLevel->stop();
		mLevel->reset();
	}
	mLevel.reset(new Level::Level());
	mLevel->init(&window(), &resource());
	mBGMusic->play();
}

void Edit::drawBaseGUI()
{
	// Alias for keys being hit.
	using Key	= sf::Keyboard;
	auto& KeyHit = Key::isKeyPressed;

	//! Main menu bar
	ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.3, 0.3, 0.6, 0.8));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.5, 0.5, 1, 1));
	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("New", "N") || KeyHit(Key::N))
		{
			newLevel();
		}
		if (ImGui::MenuItem("Load", "L") || KeyHit(Key::L))
		{
		}
		if (ImGui::MenuItem("Save", "S") || KeyHit(Key::S))
		{
		}
		if (ImGui::MenuItem("Quit", "Q") || KeyHit(Key::Q))
		{
			changeState(new Menu());
		}
		if (ImGui::MenuItem("Exit", "E") || KeyHit(Key::E))
		{
			window().close();
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("View"))
	{
		ImGui::MenuItem("Properties", nullptr, &mPropertiesPanelVisible);
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Editor"))
	{
		if (ImGui::MenuItem("Play", "P") || KeyHit(Key::P))
		{
			changeState(new Game(mLevel));
		}
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