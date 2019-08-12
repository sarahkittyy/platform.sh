#include "State/Edit.hpp"

namespace State
{

void Edit::init()
{
	mBGMusic = resource().music("assets/music/bg.flac");
	mBGMusic->setVolume(75);
	mBGMusic->setLoop(true);
	mBGMusic->play();
}

void Edit::update()
{
	// Draw GUI.
	drawPropertiesPanel();

	window().clear(sf::Color::Black);


	ImGui::SFML::Render(window());
	window().display();
}

void Edit::on(const sf::Event& event)
{
}

void Edit::drawPropertiesPanel()
{
	ImGui::Begin("Properties", nullptr);

	ImGui::End();
}

}