#version 400
in vec3 Normal;
in vec2 TexCoord;
in vec3 ViewPos;

out vec4 ColorData;

//Input textures
uniform sampler2D diffuseTex;

uniform mat4 ViewMatrix;

//Directional light
uniform vec3 dirlightDirection; // Light position in world coords.
uniform vec3 dirlightIntensity; // Diffuse intensity
uniform vec3 dirlightColor;

const int nrOfPointLights = 1;
struct Pointlight {
	vec3 Position; // Light position in world coords.
	vec3 Intensity; // Diffuse intensity
	vec3 Color;
	float Range;
}; 
uniform Pointlight pointlights[nrOfPointLights];

struct MaterialInfo {
	float Ks;
	float Shininess;
};
MaterialInfo Material;

vec3 NmNormal;

void phongModelDirLight(out vec3 ambient, out vec3 diffuse, out vec3 spec) 
{
    ambient = vec3(0.0);
    diffuse = vec3(0.0);
    spec    = vec3(0.0);

    vec3 lightVec = -normalize(( ViewMatrix*vec4(dirlightDirection, 0.0) ).xyz);

	ambient = dirlightColor * dirlightIntensity.x;

	vec3 E = normalize(ViewPos);

	float diffuseFactor = dot( lightVec, NmNormal );

	if(diffuseFactor > 0)
	{
		// diffuse
		diffuse = diffuseFactor * dirlightColor * dirlightIntensity.y;

		// specular
		vec3 v = reflect( lightVec, NmNormal );
		float specFactor = pow( max( dot(v, E), 0.0 ), Material.Shininess );
		spec = specFactor * dirlightColor * dirlightIntensity.z * Material.Ks;        
	}

	return;
}

void phongModel(int index, out vec3 ambient, out vec3 diffuse, out vec3 spec) {

    ambient = vec3(0.0);
	diffuse = vec3(0.0);
	spec    = vec3(0.0);

	vec3 lightVec = (ViewMatrix * vec4(pointlights[index].Position, 1.0)).xyz - ViewPos;
	float d = length(lightVec);

	if(d > pointlights[index].Range)
	return;
	lightVec /= d; //normalizing
        
	ambient = pointlights[index].Color * pointlights[index].Intensity.x;
	vec3 E = normalize(ViewPos);
	float diffuseFactor = dot( lightVec, NmNormal );

	if(diffuseFactor > 0)
	{
		// diffuse
		diffuse = diffuseFactor * pointlights[index].Color * pointlights[index].Intensity.y;
		// specular
		vec3 v = reflect( lightVec, NmNormal );
		float specFactor = pow( max( dot(v, E), 0.0 ), Material.Shininess );
		spec = specFactor * pointlights[index].Color * pointlights[index].Intensity.z * Material.Ks;          
	}

	float att = 1 - pow((d/pointlights[index].Range), 1.0f);

	ambient *= att;
	diffuse *= att;
	spec    *= att;

	return;
}

void main() 
{
	// Diffuse tex
	vec4 albedo_tex = texture( diffuseTex, TexCoord );

		//albedo_tex = vec4(0.8f, 0.8f, 0.8f, 1.0f);

	if(albedo_tex.a == 0.0f)
		discard;
	
	NmNormal = Normal;

	// Spec data
	Material.Ks			= 0.5f;		//specglow_map.x;
	Material.Shininess  = 80.0f;	//specglow_map.y * 254.0f + 1.0f;

	vec3 ambient = vec3(0.0);
	vec3 diffuse = vec3(0.0);
	vec3 spec    = vec3(0.0);

	if(length( dirlightIntensity ) > 0.0)
	{
		vec3 a,d,s;

		phongModelDirLight(a, d, s);
		ambient += a;
		diffuse += d;
		spec    += s;
	}

	//för varje ljus-----------
	//for(int i = 0; i < pointlights.length(); i++)
	//{
	//	vec3 a,d,s;

	//	if( length(pointlights[i].Intensity) > 0.0 )
	//	{
	//		phongModel(i, a, d, s);
	//		ambient += a;
	//		diffuse += d;
	//		spec    += s;
	//	}
	//}

	ColorData = vec4(ambient + diffuse, 1.0) * albedo_tex + vec4(spec, 0.0f);
}