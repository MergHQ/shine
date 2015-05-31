#ifndef DISABLE_COMMAND_H
#define DISABLE_COMMAND_H

#include "Command.h"
#include <string>

class DisableCommand : public Command
{
public:
	DisableCommand(std::string attribute);
	~DisableCommand();

	virtual void execute() override;
private:
	std::string m_attribute;
};

#endif