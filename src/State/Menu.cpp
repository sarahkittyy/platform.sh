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
	mTitle.setPosition(window().getSize().x / 2.f - 30, 80);
	mTitle.setRotation(-5);

	// Init the demo map.
	mDemoMap.load(&resource(), "assets/maps/demo_map.json", true);
	sf::Vector2f mapCenter;
	mapCenter.x = mDemoMap.getMapSize().x * mDemoMap.getTileSize().x;
	mapCenter.y = mDemoMap.getMapSize().y * mDemoMap.getTileSize().y;
	mDemoMap.setOrigin(mapCenter / 2.f);
	mDemoMap.setPosition(window().getSize().x / 2.f, window().getSize().y / 2.f);
	mDemoMap.setScale(2, 2);
}

void Menu::update()
{
	// Update animations.
	updateAnimations();

	// Set up all gui.
	drawGUI();

	// Start drawing
	window().clear(sf::Color::Black);
	// Draw the demo map.
	window().draw(mDemoMap);
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
	const float titlePulseSize  = 9.f;
	const float titlePulseSpeed = 4.f;
	// Scale the title by a sine wave of the elapsed time, to pulse.
	float titleScale = (std::sin(secondsElapsed * titlePulseSpeed) + titlePulseSize) / (titlePulseSize + 1);
	mTitle.setScale(titleScale, titleScale);

	//* Tilemap movement.
	const float mapMoveScale = 150.f;

	sf::Vector2f mapOffset(0, 0);
	mapOffset.x = std::sin(secondsElapsed) * mapMoveScale;
	mapOffset.y = std::cos(secondsElapsed) * mapMoveScale;

	mDemoMap.setPosition(mapOffset);

	//* Tilemap rotation.
	const float DEG2RAD		   = 3.14159f / 180.f;
	const float mapRotateSpeed = 1.f * DEG2RAD;
	float mapRotatePulse	   = std::sin(secondsElapsed) + 0.5f;

	mDemoMap.rotate(mapRotateSpeed * mapRotatePulse);

	//* Tilemap pulsing.
	const float mapPulseSize  = 13.f;
	const float mapPulseSpeed = 3.f;
	float mapScale			  = (std::cos(secondsElapsed * mapPulseSpeed) + mapPulseSize) / (mapPulseSize + 1);
	mDemoMap.setScale(mapScale, mapScale);
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
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 1));
	ImGui::Begin("/bin/sh###menu", nullptr, flags);
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	// Draw the shell.
	mShell.draw();

	ImGui::End();

	// If we should start, change state.
	if (mShell.shouldStart())
	{
		startGame();
	}
	if (mShell.shouldExit())
	{
		window().close();
	}
}

void Menu::startGame()
{
	mBGMusic->stop();
	changeState(new Game());
}

}