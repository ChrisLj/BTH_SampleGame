#include "stdafx.h"
#include "Camera.h"

Camera::Camera(int windowWidth, int windowHeight)
{
	m_pos = glm::vec3(0.f, 1.f, 3.f);
	m_up = vec3(0.f, 1.f, 0.f);
	m_right = vec3(1.f, 0.f, 0.f);
	m_look = vec3(0.f, 0.f, -1.0f);

	m_camPitch = 0.0f;
	m_camYaw = M_PI;
	m_sensitivity = 0.002f;
	m_moveSpeed = 0.015f;

	m_farPlane = 100.f;

	m_projectionMatrix = glm::perspective(45.0f, (float)windowWidth / (float)windowHeight, 0.15f, m_farPlane);
}

Camera::Camera(const Camera& _cam)
{
	m_pos		= _cam.m_pos;
	m_up		= _cam.m_up;
	m_right		= _cam.m_right;
	m_look		= _cam.m_look;

	m_camPitch = _cam.m_camPitch;
	m_camYaw = _cam.m_camYaw;
	m_sensitivity = _cam.m_sensitivity;
	m_moveSpeed = _cam.m_moveSpeed;

	m_farPlane = _cam.m_farPlane;

	m_projectionMatrix = _cam.m_projectionMatrix;
}

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::Update(float dt)
{
}

void Camera::MoveForward(float dt)
{
	m_pos += m_moveSpeed*dt*m_look;
}

void Camera::MoveBackward(float dt)
{
	m_pos -= m_moveSpeed*dt*m_look;
}

void Camera::MoveLeft(float dt)
{
	m_pos -= m_moveSpeed*dt*m_right;
}

void Camera::MoveRight(float dt)
{
	m_pos += m_moveSpeed*dt*m_right;
}

void Camera::UpdateMouse(float midX, float midY, int x, int y)
{
	m_camYaw += m_sensitivity*(midX - (float)x);
	m_camPitch += m_sensitivity*(midY - (float)y);

	m_look = vec3(cos(m_camPitch) * sin(m_camYaw), sin(m_camPitch), cos(m_camPitch)*cos(m_camYaw));
	m_right = vec3(sin(m_camYaw - M_PI / 2.0f), 0, cos(m_camYaw - M_PI / 2.0f));
	m_up = glm::cross(m_right, m_look);
}

mat4* Camera::GetViewMatrix()
{
	m_viewMatrix = glm::lookAt(m_pos, m_pos + m_look, m_up);

	return &m_viewMatrix;
}

void Camera::UpdateProjMatrix(int windowWidth, int windowHeight)
{
	m_projectionMatrix = glm::perspective(45.0f, (float)windowWidth / (float)windowHeight, 0.15f, m_farPlane);
}