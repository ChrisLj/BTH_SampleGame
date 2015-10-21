#ifndef OBJPARSER_H
#define OBJPARSER_H

#include "glm\glm.hpp"
#include <string>
#include <vector>

using glm::vec2;
using glm::vec3;

struct Vector3
{
	float val[3];
};
struct Vector2
{
	float val[2];
};

struct MeshMaterialData
{
	std::string Name;
	unsigned int Illum;
	vec3 Kd;
	vec3 Ka;
	vec3 Ks;
	vec3 Tf;
	std::string map_Kd;
	std::string map_Ks;
	std::string bump;
	std::string disp;
	std::string occlusion;
	float Ni;
	float Ns;

	MeshMaterialData()
	{
		Name = "none";
		map_Kd = "none";
		map_Ks = "none";
		bump = "none";
		//disp = "none";
		//occlusion = "none";
	}
};

struct VertexPoint
{
	vec3 position;
	vec2 texCoord;
	vec3 normal;

	VertexPoint()
	{
	}

	VertexPoint(vec3 pos, vec2 tCoord, vec3 norm)
	{
		position = pos;
		texCoord = tCoord;
		normal = norm;
	}
};

class ObjParser
{
private:
	std::vector<vec3> mPositions;
	std::vector<vec2> mTexCoords;
	std::vector<vec3> mNormals;

	std::vector<Vector3> m_finalPositions;
	std::vector<Vector2> m_finalTexCoords;
	std::vector<Vector3> m_finalNormals;

	//std::vector<VertexPoint> m_vertexPoints;

	void ltrim(std::string& str, const std::locale& loc = std::locale());
	void rtrim(std::string& str, const std::locale& loc = std::locale());
	void btrim(std::string& str, const std::locale& loc = std::locale());

	void ParseComment(std::ifstream& f);
	void ParsePosition(std::ifstream& f);
	void ParseTexCoord(std::ifstream& f);
	void ParseNormal(std::ifstream& f);

	void ParseFace(std::ifstream& f);

public:
	ObjParser(void);
	~ObjParser(void);

	bool Load(char* filename);

	//std::vector<VertexPoint>* GetVertexPoints() { return &m_vertexPoints; }
	float* GetVertexDataPtr() { return &m_finalPositions[0].val[0]; }
	float* GetTexCoordDataPtr() { return &m_finalTexCoords[0].val[0]; }
	float* GetNormalDataPtr() { return &m_finalNormals[0].val[0]; }

	unsigned int GetVertexCount() { return m_finalPositions.size(); }

};

#endif