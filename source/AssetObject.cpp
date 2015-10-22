#include "stdafx.h"
#include "AssetObject.h"
#include "ResourceManager.h"

extern ResourceManager gResourceManager;

AssetObject::AssetObject(vec3 pos, float scale, const char* textureFilepath, const char* modelFile) : WorldObject(pos, scale, textureFilepath)
{
	ModelFileParser* modelData = gResourceManager.LoadModel(modelFile);
	m_nrOfVertices = modelData->GetVertexCount();

	float* vertices		= modelData->GetVertexDataPtr();
	float* normals		= modelData->GetNormalDataPtr();
	float* texCoords	= modelData->GetTexCoordDataPtr();

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

	gResourceManager.FreeModelData(modelData);
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

