#version 400
layout( location = 0 ) in vec3 VertexPosition;
layout( location = 1 ) in vec3 VertexNormal;
layout( location = 2 ) in vec2 VertexTexCoord;
//layout( location = 3 ) in mat4 ModelViewMatrix; //for instancing

out vec3 Normal;
out vec2 TexCoord;
out vec3 ViewPos;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

void main()
{
	Normal = normalize( NormalMatrix * VertexNormal);
	TexCoord = VertexTexCoord;
	ViewPos = (ModelViewMatrix * vec4(VertexPosition, 1.0)).xyz;
	//mat4 MVP = ViewProjectionMatrix * ModelMatrix;
	gl_Position = MVP * vec4(VertexPosition, 1.0);
}