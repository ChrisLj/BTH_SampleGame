#include "stdafx.h"
#include "WorldObject.h"


WorldObject::WorldObject(vec3 pos, float scale, GLuint texture)
{
	m_position = pos;
	m_scale = scale;
	m_texture = texture;
	m_modelMatrix = mat4(1.0f);
	m_modelMatrix = glm::translate(pos);
	m_modelMatrix = glm::scale(m_modelMatrix, vec3(m_scale, m_scale, m_scale));
	
	if (m_texture == 0)
	{
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		char data[] = { 200, 200, 200, 255 };

		//gluBuild2DMipmaps(GL_TEXTURE_2D, channels, width, height, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	
	// Create vertex array object
	glGenVertexArrays(1, &m_VAO);
	//glBindVertexArray(m_VAO);

	glGenBuffers(3, m_buffers);
}


WorldObject::~WorldObject()
{
}
