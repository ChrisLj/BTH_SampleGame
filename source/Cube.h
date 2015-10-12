#ifndef CUBE_H
#define CUBE_H

#include "WorldObject.h"
class Cube : public WorldObject
{
public:
	Cube(vec3 pos, float scale, GLuint texture);
	~Cube();

	void Draw(mat4* viewMatrix, mat4* projMatrix, Shader* shader) override;
};

#endif
