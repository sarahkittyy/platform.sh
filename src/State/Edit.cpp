#include "State/Edit.hpp"

namespace State
{

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

void Edit::drawPropertiesPanel()
{
	ImGui::Begin(mPropertiesPanel.title("Properties").c_str(), nullptr);
	mPropertiesPanel.draw();
	ImGui::End();
}

}