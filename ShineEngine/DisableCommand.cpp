#include "DisableCommand.h"
#include "shine.h"

DisableCommand::DisableCommand(std::string attribute)
	: m_attribute(attribute)
{
}

DisableCommand::~DisableCommand()
{
}

void DisableCommand::execute()
{
	if (m_attribute == "GOD_MODE")
	{
		gSys->Log("GOD_MODE disabled");
	}
}
