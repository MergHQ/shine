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

	bool m_shouldReload;

	// IShader
	virtual string GetName() { return m_name; };
	virtual int GetId() { return m_id; };
	virtual bool LoadShader(string shader);
	virtual bool Reload();
	virtual GLuint& GetShaderProgramme() { return sprog; };
	virtual string GetFileName() { return m_sfile; }
	virtual void Update();
	// ~IShader

	// MVP, texture, obj2world, DepthBias, shadowmap, texture toggle.
	GLuint uniformLocations[6];

protected:
private:
	void GenerateUniformLocations();
	string m_name;
	string m_sfile;
	int time;
	int m_id;
	bool m_firstTime;
	GLuint sprog;
};
#endif