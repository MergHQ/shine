#ifndef INPUT_LISTENER_H
#define INPUT_LISTENER_H

struct GLFWwindow;

class IInputListener
{
public:
	IInputListener();
	~IInputListener();

	virtual bool key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void setIndex(int index);
protected:
	int m_index;
};
#endif