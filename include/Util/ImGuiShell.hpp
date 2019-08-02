#pragma once

#include <cctype>
#include <functional>
#include <sstream>
#include <string>
#include <unordered_map>
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
	/**
	 * @brief Program type signature
	 * 
	 * @param const std::vector<std::string>& => The command args
	 * @param std::ostream& => The output stream.
	 * 
	 */
	typedef std::function<void(const std::vector<std::string>&, std::ostream&)> Program;

	ImGuiShell();
	~ImGuiShell();

	/// Set a program's callback.
	void setProgram(std::string name, Program program);

	/// Draw to an imgui window.
	void draw();

	/// True if the game should begin.
	bool shouldStart();

private:
	/// The fake shell prompt.
	const std::string PS1;

	/// The input buffer.
	char* mInputBuf;
	/// The size of the input buffer.
	size_t mInputBufSize;
	/// Empties the buffer.
	void clearBuffer();

	/// All runnable programs.
	std::unordered_map<std::string, Program> mPrograms;

	bool mShouldStart;

	/// The saved shell history.
	std::vector<std::string> mHistory;
	/// Print a line of output.
	void println(std::string line);
	/// Saves the current line into history.
	void saveLine();

	/// The main bulk of the shell, actually running the inputted line.
	void processLine();

	/// Called for when enter is hit in the input text.
	void onEnter();

	/// Initialize all programs.
	void initShell();
};

}