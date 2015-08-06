#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_
#pragma once

#include <GL\glew.h>
#include <GL\GL.h>
#include <GLFW\glfw3.h>

class CMainWindow
{
public:
	CMainWindow();
	~CMainWindow();
	void Init();
	GLFWwindow* GetWindow() { return window; }
protected:
private:
	GLFWwindow* window;
}; 
#endif