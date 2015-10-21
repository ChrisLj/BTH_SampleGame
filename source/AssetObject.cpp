#include "stdafx.h"
#include "AssetObject.h"
#include "ObjParser.h"

AssetObject::AssetObject(vec3 pos, float scale, const char* textureFilepath) : WorldObject(pos, scale, textureFilepath)
{
	ObjParser m_parser;
	if (!m_parser.Load("C://Users/Chris/Documents/Skola/BTH_SampleGame/assets/cube.obj")) 
	{
		printf("FILE NOT LOADED! \n");
	}

	m_nrOfVertices = m_parser.GetVertexCount();

	float* vertices		= m_parser.GetVertexDataPtr();
	float* normals		= m_parser.GetNormalDataPtr();
	float* texCoords	= m_parser.GetTexCoordDataPtr();

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


AssetObject::~AssetObject()
{
}

void AssetObject::Draw(mat4* viewMatrix, mat4* projMatrix, Shader* shader)
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

