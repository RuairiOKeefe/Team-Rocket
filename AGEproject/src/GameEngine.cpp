#include "GameEngine.h"
#include <assert.h>
#include "Shader.h"
Shader *Shader::instance = 0;
GameEngine *GameEngine::instance = nullptr;

void GameEngine::Initialise()
{
	if (!glfwInit())
	{
		fprintf(stderr, "ERROR: glfw failed init! exiting.");
		return;
	}

	// Create a windowed mode window with hard coded parameters.
	instance->window = glfwCreateWindow(1920, 1080, "Team Rocket", nullptr, nullptr);
	// Window is now initalised, now make it the current context.
	glfwMakeContextCurrent(instance->window);
	if (!instance->window)
	{
		assert(instance->window != NULL);
		CleanUp();
		return;
	}

	// Set up glew.
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "ERROR: %p EXITING!", glewGetErrorString(glewInit()));
		return;
	}
	// glExperimental throws junk errors, Ignore.
	glGetError();
	PrintGlewInfo();
	LoadShaders();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glfwSwapInterval(1.0f);
}

void GameEngine::Render(glm::mat4 m, Model model, Effect effect)
{
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080, 0.1f, 1000.0f);

	// Or, for an ortho camera :
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 3, 50), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	auto mvp = instance->cameraMVP*m;
	//auto mvp = Projection*View*m;

	Shader::Instance()->UseShader("Basic", effect, mvp);

	model.Draw();
}

void GameEngine::SetCamera(glm::mat4 camera)
{
	cameraMVP = camera;
}

void GameEngine::Start()
{
	// Application is over, handle resource cleaning.
	//CleanUp();
}



void GameEngine::CleanUp()
{
	glfwTerminate();

}


void GameEngine::PrintGlewInfo()
{
	printf("-------------------------------------------------------\n");
	printf("Glew version: %p\n", glewGetString(GLEW_VERSION));
	printf("Gl version: %p\n", glGetString(GL_VERSION));
	printf("Vendor: %p\n", glGetString(GL_VENDOR));
	printf("Graphics card: %p\n", glGetString(GL_RENDERER));
	printf("Shading: %p\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	printf("-------------------------------------------------------\n");
}

void GameEngine::LoadShaders()
{
	Shader::Instance()->AddShader("Basic");
}

unsigned int GameEngine::LoadTextures(const char* location)
{
	unsigned int texture;
	// texture 1
	// ---------
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char *data = stbi_load(location, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	return texture;
}
