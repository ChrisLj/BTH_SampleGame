#ifndef QUAD_H
#define QUAD_H

#include "WorldObject.h"
class Quad : public WorldObject
{
public:
	Quad(vec3 pos, float scale, const char* textureFilepath);
	~Quad();

	void Draw(mat4* viewMatrix, mat4* projMatrix, Shader* shader) override;
};

#endif
