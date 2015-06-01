#ifndef CAMERA_INPUT_H
#define CAMERA_INPUT_H

#include "IInputListener.h"

class CCameraInput :	public IInputListener
{
public:
	CCameraInput();
	~CCameraInput();

	virtual bool key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) override;
};

#endif