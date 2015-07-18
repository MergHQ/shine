#include "ConsoleSystem.h"
#include "SetCommand.h"
#include <string>
#include <sstream>

CConsoleSystem::CConsoleSystem()
	: consoleCommands(IConsoleCommands())
{
	consoleCommands.Init();
}

CConsoleSystem::~CConsoleSystem()
{

}

void CConsoleSystem::handleCommand(const char* cmd)
{
	std::vector<std::string> words = split(cmd, ' ');
	if (words.size() > 1)
	{
		executeCommand(words);
	}
}

void CConsoleSystem::executeCommand(std::vector<std::string> words)
{
	if (words[0] == "set" && words.size() == 3)
	{
		for (unsigned int i = 0; i < consoleCommands.options.size(); i++)
		{
			Option currOpt = consoleCommands.options.at(i);
			if (words[1] == currOpt.m_name)									// Checks if option exists
			{
				for (unsigned int j = 0; j < currOpt.m_values.size(); j++)
				{
					double currVal = currOpt.m_values.at(j);
					if (std::stod(words[2]) / 100.0 == currVal)						// Checks if value exists
					{
						CSetCommand cmd;
						cmd.m_option = currOpt;
						cmd.m_value = currVal;
						cmd.execute();
						return;
					}
				}
				return;
			}
		}
	}
}

// -------- Functions for splitting strings -------- //

// Inserted vector gets modified
std::vector<std::string> &CConsoleSystem::split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

// Returns new vector
std::vector<std::string> CConsoleSystem::split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}