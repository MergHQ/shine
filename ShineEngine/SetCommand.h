#ifndef SET_COMMAND_H
#define SET_COMMAND_H

#include "Command.h"
#include "Option.h"

class CSetCommand : public Command
{
public:
	CSetCommand(Option option, double value);
	~CSetCommand();
	virtual void execute() override;
private:
	Option m_option;
	double m_value;
};

#endif