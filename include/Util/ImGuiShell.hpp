#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include "imgui/imgui-SFML.h"
#include "imgui/imgui.h"

namespace Util
{

/**
 * @brief Specialied class for the input shell on the title screen.
 * 
 */
class ImGuiShell
{
public:
	ImGuiShell();
	~ImGuiShell();

	/// Draw to an imgui window.
	void draw();

private:
	/// The fake shell prompt.
	const std::string PS1;

	/// The input buffer.
	char* mInputBuf;
	/// The size of the input buffer.
	size_t mInputBufSize;
	/// Empties the buffer.
	void clearBuffer();

	/// The saved shell history.
	std::vector<std::string> mHistory;
	/// Print a line of output.
	void println(std::string line);
	/// Saves the current line into history.
	void saveLine();

	/// Called for when enter is hit in the input text.
	void onEnter();
};

}