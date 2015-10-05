#include "stdafx.h"
#include "WorldObject.h"


WorldObject::WorldObject(vec3 pos, float scale)
{
	m_position = pos;
	m_scale = scale;
	m_modelMatrix = mat4(1.0f);
	m_modelMatrix = glm::translate(pos);

	// Create vertex array object
	glGenVertexArrays(1, &m_VAO);
	//glBindVertexArray(m_VAO);

	glGenBuffers(3, m_buffers);
}


WorldObject::~WorldObject()
{
}
