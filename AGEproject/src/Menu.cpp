#include "Menu.h"
#include "RayCast.h"
#include "GeometryUtil.h"

Menu::Menu(std::vector<char *> textureLocs) {}


void Menu::SelectionUp()
{
	if (buttons.size() == 0)
		return;
	// If currentSelection is the first one, loop to end one. 
	if (currentSelection == 0)
		currentSelection = buttons.size()-1;
	currentSelection -= 1;
}

void Menu::SelectionDown()
{
	if (buttons.size() == 0)
		return;
	// If currentSelection is the last one one, loop to first one. 
	if (currentSelection == buttons.size() - 1)
		currentSelection = 0;
	currentSelection += 1;
}

int Menu::SelectionPicked()
{
	return buttons.at(currentSelection).action;
}

int Menu::Draw(GLShader shader)
{
	int numberOfButtons = 3;
	buttons.resize(numberOfButtons);
	unsigned int tex = Texture("../res/textures/debug.png").GetTextureId();
	// 3 buttons have to fit in a screen space of two (-1 to 1).

	float buttonWidth = 0.6f;
	float buttonHeight = 0.3f;
	float buttonOffset = 0.5f;
	const float offsetChange = 0.5f;

	for (int i = 0; i < numberOfButtons; i++)
	{
		Button& newButton = buttons[i];
		newButton.action = i;
		newButton.texture = tex; 
		newButton.renderTarget = Quad(glm::vec2(0 - (buttonWidth / 2.0f), (i + buttonOffset - 1) - (buttonHeight / 2.0f)),
									  glm::vec2(0 + (buttonWidth / 2.0f), (i + buttonOffset - 1) + (buttonHeight / 2.0f)));
		newButton.renderTarget.SetOpenGL();
		buttonOffset -= offsetChange;
	}

	menu_cam->GetComponent<Menu_Camera>().Update(0);
	glm::dmat4 camMatrix = menu_cam->GetComponent<Menu_Camera>().GetProjection() * menu_cam->GetComponent<Menu_Camera>().GetView();
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 1, 0, 1);
	shader.Use();

	// Draw the quad.
	for (int i = 0; i < numberOfButtons; i++)
	{
		// Bind texture.
		glUniform1i(shader.GetUniformLocation("tex"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, buttons.at(i).texture);
		buttons[i].renderTarget.Draw();
	}
	glfwSwapBuffers(GameEngine::Get().GetWindow());

	if (UserControls::Get().IsKeyPressed(std::string("Forward")))
		SelectionUp();
	if (UserControls::Get().IsKeyPressed(std::string("Backward")))
		SelectionDown();

	if (UserControls::Get().IsMouseButtonPressed(std::string("Action")))
	{
		// "Start Game" is clicked
		if (buttons[0].renderTarget.IsMouseInBounds())
		{
			selectionMade = true;
			currentSelection = 0;
		}
		else if (buttons[1].renderTarget.IsMouseInBounds())
		{

		}
		else if (buttons[2].renderTarget.IsMouseInBounds())
		{
			selectionMade = true;
			currentSelection = 2;
		}
	}

	glfwPollEvents();

	if (selectionMade)
		return SelectionPicked();
	else
		return -1;
}
