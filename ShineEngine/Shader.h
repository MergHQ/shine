#ifndef _SHADER_H_
#define _SHADER_H_

#pragma once

#include "IShader.h"
#include "IMaterial.h"

class CShader : public IShader
{
public:
	CShader::CShader(SShaderParams* pShaderParams);
	CShader::~CShader(){};

	// IShader
	virtual const char* GetName() { return m_name; };
	virtual int GetId() { return m_id; };
	virtual bool LoadShader(const char* shader);
	virtual bool Reload();
	virtual GLuint GetShaderProgramme() { return sprog; };
	virtual const char* GetFileName() { return m_sfile; }
	virtual void Update();
	// ~IShader

	bool m_shouldReload;

private:
	const char* m_name;
	int m_id;
	const char* m_sfile;
	GLuint sprog;
	bool m_firstTime;
	int time;


};


#endif