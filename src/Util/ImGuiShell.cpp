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
}

ImGuiShell::~ImGuiShell()
{
	// Deallocate the input buffer.
	delete[] mInputBuf;
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
	println("command\noutput o/w/o");
	clearBuffer();
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

}