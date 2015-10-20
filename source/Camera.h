#ifndef CAMERA_H
#define CAMERA_H
#include "stdafx.h"

#ifndef M_PI
#define M_PI 3.14159265359
#endif

using glm::vec3; 
using glm::vec4;
using glm::mat4;
using glm::cross;
using glm::normalize;

class Camera
{
private:
	vec3 m_right, m_up, m_look, m_pos;
	float m_camPitch, m_camYaw, m_sensitivity, m_moveSpeed;
	float m_farPlane;

	mat4 m_viewMatrix, m_projectionMatrix;

public:
	Camera(int windowWidth, int windowHeight);
	Camera(const Camera& _cam);
	Camera();
	~Camera();

	void Update(float dt);

	void MoveForward(float dt);
	void MoveBackward(float dt);
	void MoveLeft(float dt);
	void MoveRight(float dt);

	void UpdateMouse(float midX, float midY, int x, int y);

	mat4* GetViewMatrix();
	mat4* GetProjMatrix() { return &m_projectionMatrix; }

	void UpdateProjMatrix(int windowWidth, int windowHeight);

	vec3*	GetRight( ) { return &m_right; }
	vec3*	GetUp() { return &m_up; }
	vec3*	GetLook() { return &m_look; }
	vec3*	GetPos() { return &m_pos; }

	float	GetFarPlane(){ return m_farPlane; }

};

#endif