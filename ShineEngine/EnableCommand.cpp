#include "EnableCommand.h"
#include "shine.h"

EnableCommand::EnableCommand(std::string attribute)
	: m_attribute(attribute)
{
}

EnableCommand::~EnableCommand()
{
}

void EnableCommand::execute()
{
	if (m_attribute == "GOD_MODE")
	{
		gSys->Log("GOD_MODE enabled");
	}
}