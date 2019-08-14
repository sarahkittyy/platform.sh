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

	// Camera control init.
	mPanning	= false;
	mMouseStart = { 0, 0 };

	// Reset the level, if it's not already set.
	if (!mLevel)
		newLevel();

	//* Create panels
	mProperties = createPanel(new Editor::GUI::State::PropsLevel(mLevel.get()),
							  "Properties");
	mObjects	= createPanel(new Editor::GUI::State::Objects(mLevel.get()),
							  "Objects");

	// Grid line init.
	mGridLinesVisible = true;
	initGridLines();
}

void Edit::update()
{
	// Draw GUI.
	drawBaseGUI();
	drawPanel(mProperties);
	drawPanel(mObjects);

	// Update mouse panning
	if (sf::Mouse::isButtonPressed(sf::Mouse::Middle) &&
		!(ImGui::IsMouseHoveringAnyWindow() && mPanning))
	{
		sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window());
		if (!mPanning)
		{
			mPanning	= true;
			mMouseStart = mousePos;
			mLevelStart = mLevel->getCameraPosition();
		}
		// Movement scaling based on viewport size.
		sf::Vector2f defaultSize = window().getDefaultView().getSize();
		sf::Vector2f currentSize = mLevel->getViewportSize();

		// Get the scaling factor.
		float scaleX = defaultSize.x / currentSize.x;
		float scaleY = defaultSize.y / currentSize.y;
		float scale  = (scaleX + scaleY) / 2.f;
		scale		 = 1.f / scale;   // Invert.

		mLevel->setCameraPosition((mMouseStart - mousePos) * scale + mLevelStart);
	}
	else
	{
		mPanning = false;
	}

	// Draw sfml.
	window().clear(sf::Color::Black);

	// Draw grid lines, if necessary.
	if (mGridLinesVisible)
	{
		sf::View moduloView = mLevel->getViewport();
		sf::Vector2i center = (sf::Vector2i)moduloView.getCenter();
		center.x += mLevel->tileSize().x / 2;
		center.y += mLevel->tileSize().y / 2;
		center.x %= mLevel->tileSize().x;
		center.y %= mLevel->tileSize().y;
		moduloView.setCenter((sf::Vector2f)center);
		window().setView(moduloView);
		window().draw(mGridLines);
	}

	// Draw the level in the back.
	window().draw(*mLevel);

	// Render ImGui
	ImGui::SFML::Render(window());
	// Finish drawing.
	window().display();
}

void Edit::on(const sf::Event& event)
{
	switch (event.type)
	{
	default:
		break;
	// Zoom the level.
	case sf::Event::MouseWheelScrolled:
	{
		int delta = event.mouseWheelScroll.delta;
		if (delta > 0)
		{
			// Zoom in.
			mLevel->setViewportSize(mLevel->getViewportSize() * 0.8f);
			// Zoom cap.
			if (mLevel->getViewportSize().x <
				(window().getDefaultView().getSize() * 0.25f).x)
			{
				mLevel->setViewportSize(window().getDefaultView().getSize() * 0.25f);
			}
		}
		else if (delta < 0)
		{
			// Zoom out.
			mLevel->setViewportSize(mLevel->getViewportSize() * 1.2f);
			// Zoom cap.
			if (mLevel->getViewportSize().x >
				(window().getDefaultView().getSize() * 2.f).x)
			{
				mLevel->setViewportSize(window().getDefaultView().getSize() * 2.f);
			}
		}
	}
	}
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
	mLevel->addObject("Player", Object::Props().set({ { "startPos", Object::Props::fromVector(sf::Vector2f(0.f, 0.f)) } }));
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
		ImGui::Separator();
		ImGui::MenuItem("Grid", nullptr, &mGridLinesVisible);
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

void Edit::initGridLines()
{
	mGridLines.setPrimitiveType(sf::Lines);

	//! Warning - code here is super messy - edit at your own risk ;;
	// Visible area size
	sf::Vector2f size = mLevel->getViewportSize();

	// Top-Left visible position.
	sf::Vector2f tl(
		mLevel->getCameraPosition().x - size.x / 2.f,
		mLevel->getCameraPosition().y - size.y / 2.f);
	// Extra spacing to ensure whole screen is covered.
	tl.x += tl.x;
	tl.y += tl.y;
	// Extra sizing to ensure window is covered.
	tl -= (sf::Vector2f)mLevel->tileSize();
	size += (sf::Vector2f)mLevel->tileSize() * 2.f;
	size *= 4.f;

	// Alignment to grid.
	tl.x = std::floor(tl.x / mLevel->tileSize().x) * mLevel->tileSize().x;
	tl.y = std::floor(tl.y / mLevel->tileSize().y) * mLevel->tileSize().y;
	tl -= sf::Vector2f(mLevel->tileSize() / 2);

	const sf::Color gridLineColor = sf::Color(255, 255, 255, 100);

	// Vertical lines.
	for (float i = tl.x; i < tl.x + size.x; i += mLevel->tileSize().x)
	{
		mGridLines.append(sf::Vertex(sf::Vector2f(i, tl.y), gridLineColor));
		mGridLines.append(sf::Vertex(sf::Vector2f(i, tl.y + size.y), gridLineColor));
	}
	// Horizontal lines.
	for (float i = tl.y; i < tl.y + size.y; i += mLevel->tileSize().y)
	{
		mGridLines.append(sf::Vertex(sf::Vector2f(tl.x, i), gridLineColor));
		mGridLines.append(sf::Vertex(sf::Vector2f(tl.x + size.x, i), gridLineColor));
	}
}

void Edit::drawPanel(Edit::Panel& panel)
{
	if (panel->visible)
	{
		ImGui::Begin(panel->machine.title(panel->name).c_str(),
					 &panel->visible,
					 panel->flags);
		panel->machine.draw();
		ImGui::End();
	}
}

Edit::Panel Edit::createPanel(Editor::GUI::State::State* initialState,
							  std::string name,
							  ImGuiWindowFlags flags)
{
	Panel ret;
	ret.reset(new Edit::_Panel());
	ret->machine.init(initialState, &window(), &resource());
	ret->visible = true;
	ret->name	= name;
	ret->flags   = flags;
	return ret;
}

}