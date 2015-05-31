#ifndef GAME_INPUT_H
#define GAME_INPUT_H

#include "InputListener.h"

class CGameInput : public IInputListener
{
public:
	CGameInput();
	~CGameInput();

	virtual bool key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) override;
private:
};

#endif