#ifndef _RENDERER_H_
#define _RENDERER_H_
#pragma once

#include "IShader.h"
#include "PostProcessor.h"
#include "LightSystem.h"
#include "ILight.h"
#include "FPCamera.h"
#include "ITexture.h"
#include "Texture.h"
#include "ShadowGen.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <vector>

#define BUFFER_OFFSET(i) ((void*)(i))

class CRenderer
{
public:
	CRenderer();
	~CRenderer();
	void Init(GLFWwindow* pWin);
	void Render();
	void ProcessFramebuffer(GLuint ShaderProg); 
	void DrawMeshes();
	void DrawShadowMap();
	void ReleaseSystems();
	GLFWwindow* GetWin() { return window; }
	CLightSystem* GetLightSystem() { return m_pLightSystem; }

protected:
private:
	float time;
	GLFWwindow* window;
	CShadowMapFBO* pSm;
	Mat44 DepthBiasMVP;
	CPostProcessor* m_postprocessor;
	CLightSystem* m_pLightSystem;
};

#endif