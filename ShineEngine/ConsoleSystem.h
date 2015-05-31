#ifndef CONSOLE_SYSTEM_H
#define CONSOLE_SYSTEM_H

#include "ConsoleCommands.h"
#include "Command.h"

class CConsoleSystem
{
public:
	CConsoleSystem();
	~CConsoleSystem();

	void handleCommand(const char* cmd);
	Command* validateCommand(std::vector<std::string> words);
private:
	IConsoleCommands consoleCommands;
	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
	std::vector<std::string> split(const std::string &s, char delim);
};
#endif