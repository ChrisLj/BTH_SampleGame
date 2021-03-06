#include "stdafx.h"
#include "Renderer.h"
#include "PoolAllocatorInterface.h"

#define MAX_OBJECTS 100

extern ResourceManager gResourceManager;

Renderer::Renderer(Camera* cam)
{
	m_camera = cam;
	m_dirLight = DirectionalLight(glm::normalize(vec3(-0.7, -1.0, -0.5)), vec3(0.2, 1.0, 0.5), vec3(1.0, 0.9, 0.9));
}

Renderer::~Renderer()
{
}

bool Renderer::Init()
{
	SDL_Init(SDL_INIT_VIDEO);

    // Share context
    SDL_GL_SetAttribute( SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

	m_window = SDL_CreateWindow(
		"An SDL2 window",                  // window title
		SDL_WINDOWPOS_UNDEFINED,           // initial x position
		SDL_WINDOWPOS_UNDEFINED,           // initial y position
		WINDOW_WIDTH,                               // width, in pixels
		WINDOW_HEIGHT,                               // height, in pixels
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE       // flags - see below
		);

	if (m_window == NULL)
	{
		SDL_Log("Could not create window: %s\n", SDL_GetError());
		return false;
	}

	m_glcontext = SDL_GL_CreateContext(m_window);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		SDL_Log("Error: %s\n", glewGetErrorString(err));
	}

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	srand(time(NULL));
	m_objectsHandle = InitializePoolAllocator(sizeof(AssetObject), MAX_OBJECTS, POOL_ALLOCATOR_DEFAULT_ALIGNMENT);

    SDL_GL_SetSwapInterval( 0 );

	return true;
}

bool Renderer::Clean()
{
	for (int i = 0; i < m_objects.size(); i++)
		pDelete(m_objectsHandle, m_objects[i]);

	ShutdownPoolAllocator(m_objectsHandle);
	SDL_DestroyWindow(m_window);
	SDL_GL_DeleteContext(m_glcontext);
	// Clean up
	SDL_Quit();
	return true;
}

void Renderer::CreateShaders()
{
	m_standardShader.InitShaderProgram();
	m_standardShader.AddShader("../shaders/vertexShader.glsl", GL_VERTEX_SHADER);
	m_standardShader.AddShader("../shaders/fragmentShader.glsl", GL_FRAGMENT_SHADER);
	m_standardShader.FinalizeShaderProgram();
}

void Renderer::CreateStuff()
{
	m_standardShader.SetUniVariable("dirlightDirection", vector3, &m_dirLight.direction);
	m_standardShader.SetUniVariable("dirlightIntensity", vector3, &m_dirLight.intensity);
	m_standardShader.SetUniVariable("dirlightColor",	 vector3, &m_dirLight.color);

	m_standardShader.CheckUniformLocation("diffuseTex", 0);
}

float spawnPointDistance = 30.f;
float squareRadius = std::sqrt(spawnPointDistance*spawnPointDistance + spawnPointDistance*spawnPointDistance);



void Renderer::Update(float dt)
{
	vec3* camPos = m_camera->GetPos();
	vec3 spawnOrigin = vec3(camPos->x, 0.0, camPos->z) + (*m_camera->GetLook()*spawnPointDistance);

    for ( int i = m_objects.size() - 1; i >= 0; i-- )
    {
        GLuint textureToBeRemoved = m_objects[ i ]->UpdateTexture();
        /*if ( textureToBeRemoved != 0 )
            mTexturesToBeDeleted.push_back( gResourceManager.DeleteTexture( textureToBeRemoved ) );*/

        if ( glm::length( *m_objects[ i ]->GetPosition() - spawnOrigin ) > squareRadius) //~root(spawnPointDistance^2 + spawnPointDistance^2)
        {
            /*if ( m_objects[ i ]->GetTexture() == 0 )
                mDeletedObjectsTextures.push_back( m_objects[ i ]->GetFutureTexture() );
			else
                mTexturesToBeDeleted.push_back( gResourceManager.DeleteTexture( m_objects[ i ]->GetTexture() ) );*/
			pDelete(m_objectsHandle, m_objects[ i ]);
			m_objects.erase( m_objects.begin() + i );
        }
        else
        {
            const float HIGHRES_DISTANCE = 8.0f;
            const float MEDIUMRES_DISTANCE = 32.0f;
            const float LOWRES_DISTANCE = 64.0f;

            float distanceFromCamera = glm::length( *m_objects[ i ]->GetPosition() - *camPos );
            if ( distanceFromCamera <= HIGHRES_DISTANCE )
                m_objects[ i ]->LoadTexture( "highres.png", 2 );
            else if ( distanceFromCamera <= MEDIUMRES_DISTANCE )
                m_objects[ i ]->LoadTexture( "mediumres.png", 1 );
            else if ( distanceFromCamera <= LOWRES_DISTANCE )
                m_objects[ i ]->LoadTexture( "lowres.png", 0 );
        }

    }
	if (m_objects.size() < MAX_OBJECTS)
	{
		int objectsToAdd = MAX_OBJECTS - m_objects.size();
		
		for (int i = 0; i < objectsToAdd; i++)
		{
			float x = (rand() % (int)(spawnPointDistance*20)) * 0.1f - spawnPointDistance;
			float z = (rand() % (int)(spawnPointDistance*20)) * 0.1f - spawnPointDistance;
			m_objects.push_back(pNew(m_objectsHandle, AssetObject, (spawnOrigin + vec3(x, 0.0f, z)), ((rand() % 50) * 0.01f + 0.2f), "lowres.png", "diamond.obj"));
		}
	}

    for ( int i = mDeletedObjectsTextures.size() - 1; i >= 0; --i )
    {
        if ( mDeletedObjectsTextures[ i ]._Is_ready() )
        {
            mTexturesToBeDeleted.push_back( gResourceManager.DeleteTexture( mDeletedObjectsTextures[ i ].get() ) );
            mDeletedObjectsTextures.erase( mDeletedObjectsTextures.begin() + i );
        }
    }
	
    for ( int i = mTexturesToBeDeleted.size() - 1; i >= 0; --i )
    {
        if ( mTexturesToBeDeleted[ i ]._Is_ready() )
        {
            mTexturesToBeDeleted[ i ].get();
            mTexturesToBeDeleted.erase( mTexturesToBeDeleted.begin() + i );
        }
    }
}

void Renderer::Render()
{
	glClearColor(0.3f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	m_standardShader.UseProgram();
	m_standardShader.SetUniVariable("ViewMatrix", mat4x4, m_camera->GetViewMatrix());

	for (auto i : m_objects)
	{
		i->Draw(m_camera->GetViewMatrix(), m_camera->GetProjMatrix(), &m_standardShader);
	}

	SDL_GL_SwapWindow(m_window);
}

SDL_Window* Renderer::GetWindow()
{
    return m_window;
}

SDL_GLContext Renderer::GetContext()
{
    return m_glcontext;
}