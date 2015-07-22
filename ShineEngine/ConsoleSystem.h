#ifndef CONSOLE_SYSTEM_H
#define CONSOLE_SYSTEM_H
#pragma once

#include "ConsoleCommands.h"
#include "Command.h"

class CConsoleSystem
{
public:
	CConsoleSystem();
	~CConsoleSystem();

	void handleCommand(const char* cmd);
	void executeCommand(std::vector<std::string> words);
protected:
private:
	IConsoleCommands consoleCommands;
};
#endif