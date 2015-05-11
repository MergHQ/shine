#ifndef _SHADER_H_
#define _SHADER_H_

#pragma once

#include "IShader.h"

class CShader : public IShader
{
public:
	CShader::CShader(SShaderParams* pShaderParams);
	CShader::~CShader(){};

	// IShader
	virtual const char* GetName() { return m_name; };
	virtual int GetId() { return m_id; };
	virtual bool LoadShader(const char* v_shader, const char* f_shader);
	virtual bool Reload();
	virtual GLuint GetShaderProgramme() { return sprog; };
	virtual const char* GetVFile() { return m_v_file; }
	virtual const char* GetFFile() { return m_f_file; }
	// ~IShader

private:
	const char* m_name;
	int m_id;
	const char* m_v_file;
	const char* m_f_file;
	GLuint sprog;
	bool m_firstTime;

};


#endif