#pragma once
#include <memory>
#include <map>
#include <GL\glew.h>
#include <GL\GL.h>
#include <assert.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include "GLShader.h"
#include "Entity.h"
#include "Texture.h"
class Shader
{
private:
	static Shader *instance;
	static std::map<std::string, GLShader> shaders;
	static std::map<std::string, unsigned int> textures;

public:
	static Shader *Instance()
	{
		if (!instance)
		{
			instance = new Shader();
		}
		return instance;
	}

	static unsigned int AddTexture(std::string name);
	static void AddShader(std::string name);
	static void UseShader(std::string name, Effect effect, glm::mat4 mvp);

	static GLShader getShader(std::string name);
};