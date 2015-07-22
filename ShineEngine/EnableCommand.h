#ifndef ENABLE_COMMAND_H
#define ENABLE_COMMAND_H
#pragma once

#include "Command.h"
#include <string>

class EnableCommand : public Command
{
public:
	EnableCommand(std::string attribute);
	~EnableCommand();
	virtual void execute() override;
protected:
private:
	std::string m_attribute;
};
#endif