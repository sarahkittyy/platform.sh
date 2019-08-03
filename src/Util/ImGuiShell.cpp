#include "Util/ImGuiShell.hpp"

namespace Util
{

ImGuiShell::ImGuiShell()
	: PS1("[platform.sh@platform.sh ~] $"),
	  mInputBufSize(50)
{
	// Allocate the input buffer.
	mInputBuf = new char[mInputBufSize];
	clearBuffer();

	// Print the initial line.
	println("Welcome! Type \"help\" for help.");

	// Set up programs.
	initShell();
}

ImGuiShell::~ImGuiShell()
{
	// Deallocate the input buffer.
	delete[] mInputBuf;
}

void ImGuiShell::setProgram(std::string name, Program program)
{
	mPrograms[name] = program;
}

void ImGuiShell::draw()
{
	// Draw history.
	for (auto& line : mHistory)
	{
		ImGui::Text("%s", line.c_str());
	}

	// Text prompt.
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
	ImGui::Text("%s", PS1.c_str());
	ImGui::PopStyleVar();
	ImGui::SameLine();
	// Input
	ImGuiInputTextFlags inputFlags =
		ImGuiInputTextFlags_EnterReturnsTrue;
	ImGui::PushItemWidth(450);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 2));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));
	if (ImGui::InputText("###stdin",
						 mInputBuf,
						 mInputBufSize,
						 inputFlags))
	{
		onEnter();
	}
	if (!ImGui::IsAnyItemActive())
		ImGui::SetKeyboardFocusHere();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
}

void ImGuiShell::onEnter()
{
	saveLine();
	processLine();
	clearBuffer();
}

void ImGuiShell::processLine()
{
	// Get the line.
	std::string line(mInputBuf);
	// Split the line at whitespace, stripping all whitespace.
	std::vector<std::string> split;
	std::string curr;
	for (auto& ch : line)
	{
		if (std::isspace(ch))
		{
			if (!curr.empty())
			{
				split.push_back(curr);
				curr = "";
			}
		}
		else
			curr += ch;
	}
	if (!curr.empty())
		split.push_back(curr);
	if (curr.size() == 0)
		return;

	//! Vars
	std::string command = split[0];
	std::vector<std::string> args;

	if (split.size() > 1)
		args.insert(args.begin(), split.begin() + 1, split.end());

	//* Command is now split

	// Search the programs for the commmand.
	auto prog = mPrograms.find(command);
	if (prog == mPrograms.end())
		return println("sh: " + command + ": command not found");

	// Run the program.
	std::stringstream standard_out;
	prog->second(args, standard_out);
	println(standard_out.str());
}

void ImGuiShell::saveLine()
{
	mHistory.push_back(PS1 + " " + std::string(mInputBuf));
}

void ImGuiShell::println(std::string line)
{
	// Split the string at \n.
	std::vector<std::string> output;
	std::string curr;
	for (auto& ch : line)
	{
		if (ch == '\n')
		{
			output.push_back(curr);
			curr = "";
		}
		else
		{
			curr += ch;
		}
	}
	output.push_back(curr);
	////////////////////////////////

	// Insert the split lines.
	mHistory.insert(mHistory.end(), output.begin(), output.end());
}

void ImGuiShell::clearBuffer()
{
	std::fill(mInputBuf, mInputBuf + mInputBufSize, 0);
}

void ImGuiShell::initShell()
{
	// Help program.
	setProgram("help", [](auto& args, std::ostream& stdout) {
		stdout << "\n-= /bin/sh help menu =-\n\n";
		stdout << "help => print this help menu\n";
		stdout << "start [level: string] => start the game\n";
		stdout << "edit => open the editor\n";
		stdout << "exit => close the game\n";
	});

	// Programs start, edit, exit set by Menu
}

}