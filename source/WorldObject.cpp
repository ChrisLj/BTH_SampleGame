#include "stdafx.h"
#include "WorldObject.h"
#include "ResourceManager.h"

extern ResourceManager gResourceManager;

WorldObject::WorldObject(vec3 pos, float scale, const char* textureFilepath )
{
	m_position = pos;
	m_scale = scale;
	m_modelMatrix = mat4(1.0f);
	m_modelMatrix = glm::translate(m_modelMatrix, pos);
	m_modelMatrix = glm::rotate(m_modelMatrix, 0.0f, vec3(0.0f, 1.0f, 0.0f));
	m_modelMatrix = glm::scale(m_modelMatrix, vec3(m_scale, m_scale, m_scale));
	
    m_texture = 0;
    m_loadingTexture = false;
    m_textureLOD = 1;
    LoadTexture( textureFilepath, 0 );

	/*if (m_texture == 0)
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
	}*/
	
	// Create vertex array object
	glGenVertexArrays(1, &m_VAO);
	//glBindVertexArray(m_VAO);

	glGenBuffers(3, m_buffers);
}


WorldObject::~WorldObject()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(3, m_buffers);
}

void WorldObject::LoadTexture( const char* textureFilepath, int textureLOD )
{
    if ( !m_loadingTexture && m_textureLOD != textureLOD )
    {
        m_futureTexture = gResourceManager.LoadTexture( textureFilepath );
        m_loadingTexture = true;
        m_textureLOD = textureLOD;
    }
}

GLuint WorldObject::UpdateTexture()
{
    if ( m_loadingTexture && m_futureTexture._Is_ready() )
    {
        GLuint prevTexture = m_texture;
        m_texture = m_futureTexture.get();
        assert( m_texture != 0 );
        m_loadingTexture = false;
        return prevTexture;
    }
    return 0;
}

const GLuint WorldObject::GetTexture() const
{
    return m_texture;
}