#include "UserControls.h"

void UserControls::BindKey(std::string &name, unsigned int key)
{
	// If action has a binding...
	if (buttonOptions.find(name) != buttonOptions.end())
	{
		// If new key is being used for something else, swap them.
		for (auto it = buttonOptions.begin(); it != buttonOptions.end(); ++it)
		{
			if (it->second == key)
			{
				// There is a match, swap and break.
				int valueToSwap = buttonOptions.find(name)->second;
				buttonOptions.find(name)->second = it->second;
				it->second = valueToSwap;
				return;
			}
		}
	}
	// Not match, just insert.
	else
		buttonOptions.insert(std::pair<std::string, unsigned int>(name, key));
}

bool UserControls::IsKeyPressed(std::string &action)
{

	auto val = buttonOptions.find(action);
	if (val == buttonOptions.end())
		return false;
	else
	{
		if (glfwGetKey(GameEngine::Get().GetWindow(), val->second) == GLFW_PRESS)
		{			
				return true;
		}
	}
	// Should never enter here but just in case.
	return false;
}

bool UserControls::IsMouseButtonPressed(std::string &action)
{
	auto val = buttonOptions.find(action);
	if (val == buttonOptions.end())
		return false;
	else
	{
		if (glfwGetMouseButton(GameEngine::Get().GetWindow(), val->second) == GLFW_PRESS)
			return true;
	}
	// Should never enter here but just in case.
	return false;
}

bool UserControls::MouseSelection(std::string action, std::vector<Button>& buttons, bool& mouseButtonHeld, int& currentSelection)
{
	if (IsMouseButtonPressed(action))
	{
		if (!mouseButtonHeld)
		{
			for (int i = 0; i < buttons.size(); i++)
			{
				if (buttons[i].renderTarget.IsMouseInBounds())
				{
					currentSelection = i;
					break;
				}
			}
			mouseButtonHeld = true;
		}
	}
	else
	{
		if (mouseButtonHeld)
		{
			mouseButtonHeld = false;
			if (currentSelection > -1)
			{
				if (buttons[currentSelection].renderTarget.IsMouseInBounds())
					return true;
				else
					currentSelection = -1;
			}
		}
	}
	return false;
}

void UserControls::ResetKeyBindings(ControllerOption options)
{
	// clear the current map of keys.
	buttonOptions.clear();
	// Set up keyboard configuration.
	if (options == KEYBOARD)
	{
		// Mouse buttons.
		buttonOptions.insert(std::pair<std::string, unsigned int>("Action", GLFW_MOUSE_BUTTON_1));
		buttonOptions.insert(std::pair<std::string, unsigned int>("Move", GLFW_MOUSE_BUTTON_2));

		// Camera movement.
		buttonOptions.insert(std::pair<std::string, unsigned int>("Forward", GLFW_KEY_W));
		buttonOptions.insert(std::pair<std::string, unsigned int>("Backward", GLFW_KEY_S));
		buttonOptions.insert(std::pair<std::string, unsigned int>("Left", GLFW_KEY_A));
		buttonOptions.insert(std::pair<std::string, unsigned int>("Right", GLFW_KEY_D));
		buttonOptions.insert(std::pair<std::string, unsigned int>("RotateLeft", GLFW_KEY_W));
		buttonOptions.insert(std::pair<std::string, unsigned int>("RotateRight", GLFW_KEY_R));
		buttonOptions.insert(std::pair<std::string, unsigned int>("ResetCamera", GLFW_KEY_SPACE));
		
		// Other?
		buttonOptions.insert(std::pair<std::string, unsigned int>("Escape", GLFW_KEY_ESCAPE));
		buttonOptions.insert(std::pair<std::string, unsigned int>("Enter", GLFW_KEY_ENTER));

		// Hotkey/ entity options. 
		buttonOptions.insert(std::pair<std::string, unsigned int>("Hold", GLFW_KEY_X));
		buttonOptions.insert(std::pair<std::string, unsigned int>("HotKey1", GLFW_KEY_1));
		buttonOptions.insert(std::pair<std::string, unsigned int>("HotKey2", GLFW_KEY_2));
		buttonOptions.insert(std::pair<std::string, unsigned int>("HotKey3", GLFW_KEY_3));

	}
	// Set up controller configuration.
	else
	{
		std::cout << "Controller not fully implemented" << std::endl;
		// I got the values for the controls here:
		// https://github.com/LWJGL/lwjgl3-wiki/wiki/2.6.3-Input-handling-with-GLFW
		buttonOptions.insert(std::pair<std::string, unsigned int>("Action", 0));
		//buttonOptions.insert(std::pair<std::string, unsigned int>("Move Forward", GLFW_KEY_W));
		//buttonOptions.insert(std::pair<std::string, unsigned int>("Move Backward", GLFW_KEY_S));
		//buttonOptions.insert(std::pair<std::string, unsigned int>("Move left", GLFW_KEY_A));
		//buttonOptions.insert(std::pair<std::string, unsigned int>("Move right", GLFW_KEY_D));
		buttonOptions.insert(std::pair<std::string, unsigned int>("Reset camera", 10));
		buttonOptions.insert(std::pair<std::string, unsigned int>("Escape", 7));

	}
}

bool UserControls::SetCursorImage()
{
	std::cout << "To be implemented!" << std::endl;
	/*unsigned char pixels[16 * 16 * 4];
	memset(pixels, 0xff, sizeof(pixels));
	GLFWimage image;
	image.width = 16;
	image.height = 16;
	image.pixels = pixels;
	GLFWcursor* cursor = glfwCreateCursor(&image, 0, 0);*/
	return false;

}

// This returns an value which corrosponds to the pixel value that is underneath the cursor.
// The main use is for bounding boxes in order to determine what object has been selected.
// It will only work if you render the bounding boxes before this is selected.
int UserControls::GetPickedColourIndexUnderMouse()
{
		GLint viewport[4]; //var to hold the viewport info
		GLdouble modelview[16]; //var to hold the modelview info
		GLdouble projection[16]; //var to hold the projection matrix info
		GLfloat winX, winY, winZ; //variables to hold screen x,y,z coordinates

		glGetDoublev(GL_MODELVIEW_MATRIX, modelview); //get the modelview info
		glGetDoublev(GL_PROJECTION_MATRIX, projection); //get the projection matrix info
		glGetIntegerv(GL_VIEWPORT, viewport); //get the viewport info

		winX = (float)mouseX;
		winY = (float)viewport[3] - (float)mouseY;
		winZ = 0;
		GLubyte bArray[4];
		glReadPixels(winX, winY, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, bArray);
		int iResult = (bArray[0]) | (bArray[1] << 8) | (bArray[2] << 16);
		return iResult;
}

void UserControls::Update()
{
	{
		// Update cursor position.
		glfwGetCursorPos(GameEngine::Get().GetWindow(), &mouseX, &mouseY);
	}
}

void UserControls::Update(Free_Camera& cam)
{
	Update();
	mouseRay.UpdateRay(cam);
}



void UserControls::HandleConsoleInput()
{
	std::cout << "Still to be implemented!" << std::endl;
}
