#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
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

	/// Object names mapped to their icon textures.
	std::unordered_map<std::string, sf::Texture*> mObjects;
};

}