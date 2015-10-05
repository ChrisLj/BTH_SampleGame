#pragma once
#include "WorldObject.h"
class Cube : public WorldObject
{
public:
	Cube(vec3 pos, float scale);
	~Cube();

	void Draw(mat4* viewMatrix, mat4* projMatrix, Shader* shader) override;
};

