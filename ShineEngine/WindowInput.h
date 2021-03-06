#ifndef WINDOW_INPUT_H
#define WINDOW_INPUT_H
#pragma once

#include "IInputListener.h"

class CWindowInput : public IInputListener
{
public:
	CWindowInput();
	~CWindowInput();

	virtual bool key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) override;
protected:
private:
};
#endif