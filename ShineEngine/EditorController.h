#ifndef _EDITORCONTROLLER_H_
#define _EDITORCONTROLLER_H_

#pragma once

#include "IInputListener.h"
#include "IShader.h"
#include "IMesh.h"

class CEditorController : public IInputListener
{
public:
	CEditorController();
	~CEditorController();

	// IInputListener
	virtual bool key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) { return false; };
	// ~IInputListener

	void Init();
	void DrawEditorObjects();

private:
	IShader* m_pHelperShader;
	IMesh* m_pSphere;


};

#endif