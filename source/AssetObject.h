#ifndef ASSETOBJECT_H
#define ASSETOBJECT_H

#include "WorldObject.h"

class AssetObject : public WorldObject
{
public:
	AssetObject(vec3 pos, float scale, const char* textureFilepath);
	~AssetObject();

	void Draw(mat4* viewMatrix, mat4* projMatrix, Shader* shader) override;
};

#endif
