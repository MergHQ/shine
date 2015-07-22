#ifndef COMMAND_H
#define COMMAND_H
#pragma once

class Command
{
public:
	Command(){}
	~Command(){}
	virtual void execute(){}
protected:
private:
};
#endif