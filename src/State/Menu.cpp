#include "State/Menu.hpp"

namespace State
{

Menu::Menu()
{
	mClock.restart();
}

void Menu::init()
{
	// Music initialization
	mBGMusic = resource().music("assets/music/menu.flac");
	mBGMusic->setVolume(75);
	mBGMusic->setLoop(true);
	mBGMusic->play();

	// Title text initialization.
	mTitle.setFont(*resource().font("assets/fonts/karmatic-arcade.ttf"));
	mTitle.setCharacterSize(48);
	mTitle.setOutlineColor(sf::Color::White);
	mTitle.setOutlineThickness(2);
	mTitle.setFillColor(sf::Color::Black);
	mTitle.setStyle(sf::Text::Style::Bold);
	mTitle.setString("platform.sh");
	mTitle.setOrigin(mTitle.getLocalBounds().width / 2.f, 0);
	mTitle.setPosition(window().getSize().x / 2.f - 30, 40);
	mTitle.setRotation(-5);
}

void Menu::update()
{
	// Update animations.
	updateAnimations();

	// Set up all gui.
	drawGUI();

	// Start drawing
	window().clear(sf::Color::Black);
	// Draw the title.
	window().draw(mTitle);

	// Render ImGui
	ImGui::SFML::Render(window());
	window().display();
}

void Menu::updateAnimations()
{
	float secondsElapsed = mClock.getElapsedTime().asSeconds();

	//* Title pulsing
	const float pulseSize  = 9.f;
	const float pulseSpeed = 4.f;
	// Scale the title by a sine wave of the elapsed time, to pulse.
	float titleScale = (std::sin(secondsElapsed * pulseSpeed) + pulseSize) / (pulseSize + 1);
	mTitle.setScale(titleScale, titleScale);
}

void Menu::drawGUI()
{
	// Main gui window flags.
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize |
							 ImGuiWindowFlags_NoMove |
							 ImGuiWindowFlags_NoSavedSettings |
							 ImGuiWindowFlags_NoResize |
							 ImGuiWindowFlags_NoCollapse;

	// Window positioning and size.
	ImVec2 halfWindow = ImVec2(window().getSize().x, window().getSize().y / 2.f);
	ImGui::SetNextWindowPos(ImVec2(0, halfWindow.y));
	ImGui::SetNextWindowSize(ImVec2(halfWindow.x * 2.f, halfWindow.y));
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.43f, 0.43f, 0.53f, 0.8f));
	ImGui::Begin("/bin/sh###menu", nullptr, flags);
	ImGui::PopStyleColor();

	// Draw the shell.
	mShell.draw();

	ImGui::End();

	// If we should start, change state.
	if (mShell.shouldStart())
	{
		changeState(new Game());
	}
}

}