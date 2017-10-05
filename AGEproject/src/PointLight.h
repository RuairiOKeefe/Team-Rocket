#pragma once
#include "Shader.h"

class PointLight : public Component
{
	void bind(const PointLight& pointLight, const std::string& name);
	void bind(const std::vector<PointLight>& pointLights, const std::string& name);
	float range;
public:
	glm::vec3 position;

	float constant;
	float linear;
	float quadratic;

	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;	

	PointLight();
	PointLight(const glm::vec3 position, const glm::vec4 diffuse);

	~PointLight();

	void Render() override;
};

