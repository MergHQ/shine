#ifndef CONSOLE_COMMANDS_H
#define CONSOLE_COMMANDS_H

#include <vector>
#include "Option.h"

class IConsoleCommands
{
public:
	IConsoleCommands(){}
	~IConsoleCommands(){}

	std::vector<std::string> commands;
	std::vector<std::string> attributes;
	std::vector<Option> options;

	void Init()
	{
		// ---------- Commands --------------- //
		commands.push_back("set");
		commands.push_back("enable");
		commands.push_back("disable");

		// ---------- Attributes ------------- //
		attributes.push_back("GOD_MODE");

		// ---------- Options ---------------- //
		Option camera_mode("camera_mode");
		std::vector<double> mode_values = { 0, 1 };
		camera_mode.setValues(mode_values);
		options.push_back(camera_mode);
		
		Option camera_speed("camera_speed");
		std::vector<double> speed_values;
		for (unsigned int i = 0; i <= 10; i++) // 0 -> 0.1
		{
			speed_values.push_back(i / 100.0);
		}
		camera_speed.setValues(speed_values);
		options.push_back(camera_speed);
	}

};

#endif