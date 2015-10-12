#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H
#include "stdafx.h"
#include "Shader.h"

using glm::vec3;
using glm::mat3;
using glm::mat4;

class WorldObject
{
public:
	WorldObject(vec3 pos, float scale, GLuint texture);
	~WorldObject();

	vec3* GetPosition() { return &m_position; }

	virtual void Draw(mat4* viewMatrix, mat4* projMatrix, Shader* shader) = 0;

protected:
	vec3 m_position;
	float m_scale;

	mat4 m_modelMatrix, m_modelViewMatrix, m_MVP;
	mat3 m_normalMatrix;

	GLuint m_VAO;
	GLuint m_buffers[3]; //currently vertex, normal, texCoord
	unsigned int m_nrOfVertices;

	GLuint m_texture;
};

#endif