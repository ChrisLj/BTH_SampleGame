#ifndef SHADER_H
#define SHADER_H

#include "stdafx.h"
#include <vector>

struct ShaderInfo
{
	GLenum type;
	const char* file;
};

enum VariableTyp
{
	mat4x4,
	mat3x3,
	vector3,
	vector4,
	glfloat,
	glint
};

class Shader
{
public:
	void	InitShaderProgram();
	bool	AddShader(const char* source_file, GLenum shader_type);
	bool	FinalizeShaderProgram();

	void	UseProgram();

	bool    SetUniVariable(const char* p_variableName, VariableTyp p_variableType, void* p_Value);

	void	CheckUniformLocation(const char* _uniformName, int _unitLocation);

	GLuint  GetShaderProgram();

private:
	GLuint m_shaderProg;
	std::vector<GLuint>	m_shaders;
};

#endif