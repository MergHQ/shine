#include "SetCommand.h"
#include "shine.h"
#include "Camera.h"
#include "ICamera.h"

CSetCommand::CSetCommand()
{

}

CSetCommand::~CSetCommand()
{

}

void CSetCommand::execute()
{
	if (m_option.m_name == "camera_mode")
	{
		if (m_value == 0)
			gSys->GetCamera()->SetCameraMode(ICamera::CameraMode::GAME);
		else if (m_value == 1)
			gSys->GetCamera()->SetCameraMode(ICamera::CameraMode::EDITOR);
	}
	else if (m_option.m_name == "camera_speed")
	{
		gSys->GetCamera()->SetCameraSpeed(m_value);
	}
}