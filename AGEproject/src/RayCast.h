#pragma once
#include "glm\common.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "Free_Camera.h"
#include "Menu_Camera.h"
class RayCast
{
public:
	// Ray is just a position and a direction.
	glm::vec3 origin;
	glm::vec3 direction;
	glm::vec3 near;
	glm::vec3 far;
	RayCast() {}

	RayCast(Free_Camera& camera)
	{
		UpdateRay(camera);
	}
	RayCast(glm::vec3 o, glm::vec3 dir)
	{
		origin = o;
		direction = dir;
	}

	void UpdateRay(Free_Camera& camera);

	void UpdateRay(Menu_Camera& camera);

};