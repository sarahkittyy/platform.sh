#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Editor/GUI/State/State.hpp"
#include "Level/Level.hpp"
#include "Object/Objects.hpp"

namespace Editor::GUI::State
{

class Objects : public State
{
public:
	Objects(Level::Level* level);

	void init();
	void draw();

	std::string title() const;

private:
	/// The level this gui panel edits.
	Level::Level* mLevel;

	/// Structure that stores an object template, as well a pointer to it's icon texture.
	struct ObjectButton
	{
		std::shared_ptr<Object::Object> sample;
		sf::Texture* texture;
	};

	/// A vector of all bundled ObjectButton instances.
	std::vector<ObjectButton> mObjects;
};

}