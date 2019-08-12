#include "State/Edit.hpp"

namespace State
{

Edit::Edit()
{
}

Edit::Edit(std::shared_ptr<Level::Level> level)
{
	mLevel = level;
}

Edit::~Edit()
{
	mBGMusic->stop();
}

void Edit::init()
{
	mBGMusic = resource().music("assets/music/bg.flac");
	mBGMusic->setVolume(75);
	mBGMusic->setLoop(true);
	mBGMusic->play();

	/// Reset the level, if it's not already set.
	if (!mLevel)
		newLevel();

	//* Create panels
	// clang-format off
	mProperties = createPanel(new Editor::GUI::State::PropsLevel(
		Object::Props().set({
			{"levelText", mLevel->getDisplayText()},
			{"tickrate", mLevel->getTickSpeed().asSeconds()}
		})), 
		"Properties");
	// clang-format on
	mObjects = createPanel(new Editor::GUI::State::Objects(), "Objects");
}

void Edit::update()
{
	// Draw GUI.
	drawBaseGUI();
	drawPanel(mProperties);
	drawPanel(mObjects);

	// Update content from panels
	updateFromPanels();

	// Draw sfml.
	window().clear(sf::Color::Black);

	// Draw the level in the back.
	window().draw(*mLevel);

	// Render ImGui
	ImGui::SFML::Render(window());
	// Finish drawing.
	window().display();
}

void Edit::updateFromPanels()
{
	mLevel->setDisplayText(mProperties->machine.getProps().get("/levelText"_json_pointer));
	mLevel->setTickSpeed(sf::seconds(mProperties->machine.getProps().get("/tickrate"_json_pointer)));
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

void Edit::loadFromFile(std::string file)
{
	newLevel();

	/// Read the file to json.
	nlohmann::json data;
	std::ifstream level(file);
	if (!level) throw std::runtime_error("Could not open file " + file + " for reading.");
	level >> data;
	level.close();

	mLevel->deserialize(data);
}

void Edit::saveToFile(std::string file)
{
	std::ofstream level(file);
	if (!level) throw std::runtime_error("Could not open file " + file + " for writing.");
	level << mLevel->serialize();
	level.close();
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
			auto selection = pfd::open_file("Open the level file..",
											"",
											{ "JSON Files", "*.json" })
								 .result()
								 .front();
			if (!selection.empty())
			{
				loadFromFile(selection);
			}
		}
		if (ImGui::MenuItem("Save", "S") || KeyHit(Key::S))
		{
			auto selection = pfd::save_file("Save to where?",
											"",
											{ "JSON Files", "*.json" })
								 .result();
			if (!selection.empty())
			{
				saveToFile(selection);
			}
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
		ImGui::MenuItem("Properties", nullptr, &mProperties->visible);
		ImGui::MenuItem("Objects", nullptr, &mObjects->visible);
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Editor"))
	{
		if (ImGui::MenuItem("Test", "T") || KeyHit(Key::T))
		{
			changeState(new Game(mLevel, true));
		}
		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
	ImGui::PopStyleColor(2);
}

void Edit::drawPanel(Edit::Panel& panel)
{
	if (panel->visible)
	{
		ImGui::Begin(panel->machine.title(panel->name).c_str(), &panel->visible);
		panel->machine.draw();
		ImGui::End();
	}
}

Edit::Panel Edit::createPanel(Editor::GUI::State::State* initialState, std::string name)
{
	Panel ret;
	ret.reset(new Edit::_Panel());
	ret->machine.init(initialState, &window(), &resource());
	ret->visible = true;
	ret->name	= name;
	return ret;
}

}