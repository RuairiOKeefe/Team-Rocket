#include "GameEngine.h"
#include "Model.h"
#include <assert.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "UserControls.h"
GameEngine *GameEngine::instance = 0;


void GameEngine::Initialise()
{
	if (!glfwInit())
	{
		std::fprintf(stderr, "ERROR: glfw failed init! exiting.");
		return;
	}

	// Create a windowed mode window with hard coded parameters.
	instance->window = glfwCreateWindow(1920, 1080, "Team Rocket", NULL, NULL);
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
		std::fprintf(stderr, "ERROR: %s EXITING!", glewGetErrorString(glewInit()));
		return;
	}
	// glExperimental throws junk errors, Ignore.
	glGetError();
	PrintGlewInfo();
	LoadShaders();
}

void GameEngine::Render()
{

	UserControls controls;
	controls.ResetKeyBindings(UserControls::KEYBOARD);
	Model model("res/models/Torus2.obj");
	// Swap the window buffers.
	glfwSwapBuffers(instance->window);
	// Clear the opengl buffer.
	glClear(GL_COLOR_BUFFER_BIT);
	controls.UpdateMousePosition();
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080, 0.1f, 100.0f);

	// Or, for an ortho camera :
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(90, 0, 90), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	if (glfwGetMouseButton(Instance()->GetWindow(), controls.GetKey("Action")))
	{
		GLShader helloShader;
		if (!helloShader.AddShaderFromFile("res/shaders/color.vert", GLShader::VERTEX))
			std::printf("Vert failed to compile.\n");
		if (!helloShader.AddShaderFromFile("res/shaders/color.frag", GLShader::FRAGMENT))
			std::printf("Frag failed to compile.\n");
		helloShader.Link();
		helloShader.Use();
		glUniformMatrix4fv(helloShader.GetUniformLocation("matrices.projectionMatrix"), 1, GL_FALSE, glm::value_ptr(Projection));
		glUniformMatrix4fv(helloShader.GetUniformLocation("matrices.viewMatrix"), 1, GL_FALSE, glm::value_ptr(View));
		glUniformMatrix4fv(helloShader.GetUniformLocation("matrices.modelMatrix"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0)));
		glm::vec4 vRed(1.0f, 0.0f, 0.0f, 1.0f); // Red color
		glUniform4fv(helloShader.GetUniformLocation("vColor"), 1, glm::value_ptr(vRed));
		//std::printf("Checking for pixel");
		int i = controls.GetPickedColorIndexUnderMouse();
		std::printf("%i \n", i);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	}
//	std::printf("-------------------------------\n");
//	std::printf("Testing Model loading\n");
	GLShader helloShader;
	if (!helloShader.AddShaderFromFile("res/shaders/BasicVert.vert", GLShader::VERTEX))
		std::printf("Vert failed to compile.\n");
	if (!helloShader.AddShaderFromFile("res/shaders/BasicFrag.frag", GLShader::FRAGMENT))
		std::printf("Frag failed to compile.\n");
	helloShader.Link();
	helloShader.Use();


	auto mvp = Projection*View*glm::mat4(1.0);

	glUniformMatrix4fv(helloShader.GetUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
	model.Draw(helloShader);
//	std::printf("-------------------------------\n");
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// process events.
	glfwPollEvents();
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
	std::printf("-------------------------------------------------------\n");
	std::printf("Glew version: %s\n", glewGetString(GLEW_VERSION));
	std::printf("Gl version: %s\n", glGetString(GL_VERSION));
	std::printf("Vendor: %s\n", glGetString(GL_VENDOR));
	std::printf("Graphics card: %s\n", glGetString(GL_RENDERER));
	std::printf("Shading: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	std::printf("-------------------------------------------------------\n");
}

void GameEngine::LoadShaders()
{
	std::printf("-------------------------------\n");
	std::printf("Testing shaders\n");
	GLShader helloShader;
	if (!helloShader.AddShaderFromFile("res/shaders/HelloWorld.vert", GLShader::VERTEX))
		std::printf("Vert failed to compile.\n");
	if (!helloShader.AddShaderFromFile("res/shaders/HelloWorld.frag", GLShader::FRAGMENT))
		std::printf("Frag failed to compile.\n");
	helloShader.Link();
	std::printf("-------------------------------\n");
}

void GameEngine::ImSorryOrHowILearnedToStopCaringAndLoadTextures()
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
	unsigned char *data = stbi_load("../res/textures/debug.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data); //Now remember to do something with this texture we got
}
