#ifndef SET_COMMAND_H
#define SET_COMMAND_H
#pragma once

#include "Command.h"
#include "Option.h"

class CSetCommand : public Command
{
public:
	CSetCommand();
	~CSetCommand();

	Option m_option;
	double m_value;
protected:
public:
	virtual void execute() override;
};
#endif