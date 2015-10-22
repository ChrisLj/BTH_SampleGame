#ifndef RENDERER_H
#define RENDERER_H

#include "stdafx.h"
#include "SDL/SDL.h"
#include "Shader.h"
#include "Camera.h"
#include <vector>
#include "WorldObject.h"
#include "Cube.h"
#include "Quad.h"
#include "AssetObject.h"
#include "ResourceManager.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

using glm::vec3;

struct DirectionalLight
{
	vec3 direction;
	vec3 intensity;
	vec3 color;
	DirectionalLight(){}
	DirectionalLight(vec3 dir, vec3 intens, vec3 col) 
		: direction(dir), intensity(intens), color(col){}
};

struct TextureHandle
{
    std::future<GLuint> future;
    int index;

    TextureHandle( size_t index, std::future<GLuint> future )
    {
        this->index = index;
        this->future = std::move(future);
    }
};

class Renderer
{
private:
	SDL_Window* m_window;
	SDL_GLContext m_glcontext;
	Shader m_standardShader;
	Camera* m_camera;
	char m_objectsHandle;
	std::vector<WorldObject*> m_objects; //custom allocator here?
	DirectionalLight m_dirLight;
    std::vector<std::future<GLuint>> mDeletedObjectsTextures;
    std::vector<std::future<void>> mTexturesToBeDeleted;

public:
	Renderer(){};
	Renderer(Camera* cam);
	~Renderer();

	bool Init();
	bool Clean();
	void CreateShaders();
	void CreateStuff();
	void Update(float dt);
	void Render();

    SDL_Window* GetWindow();
    SDL_GLContext GetContext();
};

#endif