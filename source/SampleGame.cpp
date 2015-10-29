#include "stdafx.h"
#include "Renderer.h"
#include "SoundEngine.h"
#include <chrono>
#include <future>

#include <zzip/zzip.h>

#include "ResourceManager.h"
ResourceManager gResourceManager;

SDL_Event m_event;
//Renderer m_renderer;
Camera m_camera;
SoundEngine m_soundEngine;
bool m_running;

bool wPressed = false;
bool sPressed = false;
bool aPressed = false;
bool dPressed = false;
bool tPressed = false;

void CheckInput(float dt)
{
	SDL_PollEvent(&m_event);
	switch (m_event.type)
	{
		case SDL_KEYDOWN:
			if (m_event.key.keysym.sym		== SDLK_w)
				wPressed = true;
			else if (m_event.key.keysym.sym == SDLK_s)
				sPressed = true;

			if (m_event.key.keysym.sym		== SDLK_a)
				aPressed = true;
			else if (m_event.key.keysym.sym == SDLK_d)
				dPressed = true;

			if (m_event.key.keysym.sym == SDLK_KP_PLUS)
				m_soundEngine.ChangeVolume(0.1f);
			else if (m_event.key.keysym.sym == SDLK_KP_MINUS)
				m_soundEngine.ChangeVolume(-0.1f);

			if (m_event.key.keysym.sym == SDLK_ESCAPE)
			{
				m_running = false;
			}
		break;
		case SDL_KEYUP:
			if (m_event.key.keysym.sym == SDLK_w)
				wPressed = false;
			else if (m_event.key.keysym.sym == SDLK_s)
				sPressed = false;

			if (m_event.key.keysym.sym == SDLK_a)
				aPressed = false;
			else if (m_event.key.keysym.sym == SDLK_d)
				dPressed = false;
		break;
	}

	if (wPressed)
		m_camera.MoveForward(dt);
	else if (sPressed)
		m_camera.MoveBackward(dt);
	if (aPressed)
		m_camera.MoveLeft(dt);
	else if (dPressed)
		m_camera.MoveRight(dt);
}

#undef main
int main(int argc, char* argv[])
{
	m_camera = Camera(WINDOW_WIDTH, WINDOW_HEIGHT);
	m_soundEngine.Init();
	Renderer m_renderer = Renderer(&m_camera);
	
	m_renderer.Init();
	m_renderer.CreateShaders();

    gResourceManager.StartUp( m_renderer.GetWindow(), 4, "../assets/dip.zip" );
    SDL_GL_MakeCurrent( m_renderer.GetWindow(), m_renderer.GetContext() );

	m_renderer.CreateStuff(); //Tmp. Some post init-stuff

	m_running = true;
	std::chrono::steady_clock::time_point start, end;
	start = std::chrono::high_resolution_clock::now();
	long long duration;
	float dt; //milliseconds
	
	/* ANDERS TEMP SHIT */
	ZZIP_DIR* dir = zzip_dir_open("../assets/dip.zip", 0);
	if (dir) {
		ZZIP_DIRENT dirent;
		while (zzip_dir_read(dir, &dirent))
		{
			printf("%s %i/%i\n", dirent.d_name, dirent.d_csize, dirent.st_size);
		}
	}
	/* END OF ANDERS TEMP SHIT*/

	while (m_running)	//Le loop
	{
		end = std::chrono::high_resolution_clock::now();
		duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
		start = end;
		dt = duration * 0.000001f;

		if (dt > 30.0f)
			SDL_Log("dt = %f ms", dt);

		CheckInput(dt);
		m_renderer.Update(dt);
		m_soundEngine.Update(dt);

		m_renderer.Render();
	}

    gResourceManager.ShutDown();

	m_renderer.Clean();
	m_soundEngine.Clean();

	return 0;
}

