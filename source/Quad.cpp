#include "stdafx.h"
#include "Quad.h"


Quad::Quad(vec3 pos, float scale, const char* textureFilepath ) : WorldObject(pos, scale, textureFilepath)
{
#pragma region data
	m_nrOfVertices = 6;

	float vertices[] = {
		// Front face
		-1.0f,  1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f
	};

	const float normals[] = {
		// Front face
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f
	};

	const float texCoords[] = {
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f
	};

#pragma endregion buffer_data

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, m_nrOfVertices * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, m_nrOfVertices * 3 * sizeof(GLfloat), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, m_nrOfVertices * 2 * sizeof(GLfloat), texCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
}


Quad::~Quad()
{
}

void Quad::Draw(mat4* viewMatrix, mat4* projMatrix, Shader* shader)
{
	m_modelViewMatrix = (*viewMatrix) * m_modelMatrix;
	shader->SetUniVariable("ModelViewMatrix", mat4x4, &m_modelViewMatrix);

	m_normalMatrix = glm::transpose(glm::inverse(mat3(m_modelViewMatrix)));
	shader->SetUniVariable("NormalMatrix", mat3x3, &m_normalMatrix);

	m_MVP = (*projMatrix) * m_modelViewMatrix;
	shader->SetUniVariable("MVP", mat4x4, &m_MVP);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, m_nrOfVertices);
}
