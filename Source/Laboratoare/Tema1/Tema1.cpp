#include "Tema1.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Object2D.h"
#include "Logic.h"

using namespace std;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);
	glm::vec3 corner = glm::vec3(0, 0, 0);

	
	// initialize the logic
	bricksX = 12;
	bricksY = 10;
	logic = Logic(bricksX, bricksY);

	// the width of the border
	logic.borderWidth = 30.0f;

	// the dimensions of the platform
	logic.platformWidth = 300.0f;
	logic.platformHeight = logic.borderWidth / 2; // proportional with the border for a good aspect

	// parameters of the ball
	logic.ballRadius = 10.0f;
	int circlePoints = 12;
	logic.ballSpeed = 250.0f;
	logic.ballSpeedX = 0.0f;
	logic.ballSpeedY = logic.ballSpeed;

	// parameters of the powerups squares
	float powerupSide = 20.0f;
	logic.powerupSpinSpeed = 1.5f;
	logic.powerupDropSpeed = 40.0f;
	
	//powerup1 adds a timed border at the bottom of the screen(at the same level as the platform's bottom edge)
	logic.powerup1Time = 0.0f;
	logic.powerup1TimeMax = 300.0f;
	logic.powerup1Speed = 50.0f;

	//powerup2 makes the ball strong
	logic.powerup2Time = 0.0f;
	logic.powerup2TimeMax = 300.0f;
	logic.powerup2Speed = 50.0f;
	
	// game parameters
	logic.windowX = resolution.x;
	logic.windowY = resolution.y;
	logic.initialLifes = 3;
	logic.maxLifes = 5;
	logic.gameState = 0;

	float distanceX, distanceY, spaceX, spaceY;
	distanceX = 150.0f; // the empty space left and right of the bricks
	distanceY = 150.0f; // the empty space above and below of the bricks
	// the empty space between the bricks
	spaceX = 16.0f;
	spaceY = 16.0f;

	// the dimensions of the bricks calculated based on the space that is required to be empty and the resolution
	logic.brickWidth = (resolution.x - logic.borderWidth - distanceX - logic.borderWidth - distanceX - spaceX * (bricksX - 1)) / bricksX;
	logic.brickHeight = (resolution.y - logic.borderWidth - distanceY - 2 * logic.platformHeight - distanceY - spaceY * (bricksY - 1)) / bricksY;

	// the speed of a brick breaking
	logic.brickScaleSpeed = 3.0f;

	// the starting positions of the bricks matrix
	float startX, startY;
	startX = logic.borderWidth + distanceX + logic.brickWidth / 2;
	startY = 2 * logic.platformHeight + distanceY + logic.brickHeight / 2;

	// populating the position matrixes
	for (int i = 0; i < bricksY; i++) {
		for (int j = 0; j < bricksX; j++) {
			logic.bricks.posX[i][j] = startX + j * (logic.brickWidth + spaceX);
			logic.bricks.posY[i][j] = startY + i * (logic.brickHeight + spaceY);
			logic.bricks.powerupY[i][j] = startY + i * (logic.brickHeight + spaceY);
		}
	}

	// add the borders to the list
	Mesh* leftBorder = Object2D::CreateRectangle("leftBorder", corner, logic.borderWidth, resolution.y - logic.borderWidth - logic.platformHeight, glm::vec3(1, 0, 0), true);
	AddMeshToList(leftBorder);
	Mesh* rightBorder = Object2D::CreateRectangle("rightBorder", corner, logic.borderWidth, resolution.y - logic.borderWidth - logic.platformHeight, glm::vec3(1, 0, 0), true);
	AddMeshToList(rightBorder);
	Mesh* topBorder = Object2D::CreateRectangle("topBorder", corner, resolution.x, logic.borderWidth, glm::vec3(1, 0, 0), true);
	AddMeshToList(topBorder);
	Mesh* bottomBorder = Object2D::CreateRectangle("bottomBorder", corner, resolution.x, logic.borderWidth / 2, glm::vec3(0, 0, 1), true);
	AddMeshToList(bottomBorder);

	// add the platform to the list
	Mesh* platform = Object2D::CreateRectangle("platform", corner, logic.platformWidth, logic.platformHeight, glm::vec3(0, 1, 0), true);
	AddMeshToList(platform);

	// add the ball to the list
	Mesh* ball = Object2D::CreateCircle("ball", corner, logic.ballRadius, circlePoints, glm::vec3(1, 1, 1), true);
	AddMeshToList(ball);
	Mesh* strongBall = Object2D::CreateCircle("strongBall", corner, logic.ballRadius, circlePoints, glm::vec3(1, 1, 0), true);
	AddMeshToList(strongBall);

	Mesh* brick = Object2D::CreateBrick("brick", corner, logic.brickWidth, logic.brickHeight, glm::vec3(0, 1, 1), true);
	AddMeshToList(brick);

	// add powerups to the list
	Mesh* powerup = Object2D::CreateBrick("powerup", corner, powerupSide, powerupSide, glm::vec3(0, 0, 1), true);
	AddMeshToList(powerup);
	Mesh* powerup2 = Object2D::CreateBrick("powerup2", corner, powerupSide, powerupSide, glm::vec3(1, 0, 1), true);
	AddMeshToList(powerup2);
	Mesh* powerup3 = Object2D::CreateBrick("powerup3", corner, powerupSide, powerupSide, glm::vec3(1, 1, 0), true);
	AddMeshToList(powerup3);
}

void Tema1::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{
	// get the resolution
	glm::ivec2 resolution = window->GetResolution();

	// create the borders
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(0, 2 * logic.platformHeight);
	RenderMesh2D(meshes["leftBorder"], shaders["VertexColor"], modelMatrix);
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(resolution.x - logic.borderWidth, 2 * logic.platformHeight);
	RenderMesh2D(meshes["rightBorder"], shaders["VertexColor"], modelMatrix);
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(0, resolution.y - logic.borderWidth);
	RenderMesh2D(meshes["topBorder"], shaders["VertexColor"], modelMatrix);
	

	// create the platform
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(platformX - logic.platformWidth / 2, logic.platformHeight);
	RenderMesh2D(meshes["platform"], shaders["VertexColor"], modelMatrix);

	
	

	




	// create the ball
	modelMatrix = glm::mat3(1);
	if (logic.gameState == 0) {
		// ball still atatched to the platform

		logic.ballX = platformX;
		logic.ballY = 2 * logic.platformHeight + logic.ballRadius;
		
		logic.Loop(platformX, deltaTimeSeconds);
		
		modelMatrix *= Transform2D::Translate(logic.ballX, logic.ballY);
	}
	else {
		// ball in game
		logic.Loop(platformX, deltaTimeSeconds);
		
		modelMatrix *= Transform2D::Translate(logic.ballX, logic.ballY);
	}
	if (logic.powerup2Time > 0) {
		RenderMesh2D(meshes["strongBall"], shaders["VertexColor"], modelMatrix);
	}
	else {
		RenderMesh2D(meshes["ball"], shaders["VertexColor"], modelMatrix);
	}

	if (logic.powerup1Time > 0) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(0, logic.borderWidth / 2 * (1 - logic.powerup1Time / logic.powerup1TimeMax));
		modelMatrix *= Transform2D::Scale(1, logic.powerup1Time / logic.powerup1TimeMax);
		RenderMesh2D(meshes["bottomBorder"], shaders["VertexColor"], modelMatrix);
	}
	

	// create the logic.lifes left
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(logic.borderWidth - logic.ballRadius, 2 * logic.platformHeight + logic.ballRadius + 5.0f);
	for (int i = 0; i < logic.lifes; ++i) {
		modelMatrix *= Transform2D::Translate(2 * logic.ballRadius + 5.0f, 0.0f);
		if (logic.powerup2Time > 0) {
			RenderMesh2D(meshes["strongBall"], shaders["VertexColor"], modelMatrix);
		}
		else {
			RenderMesh2D(meshes["ball"], shaders["VertexColor"], modelMatrix);
		}
	}

	for (int i = 0; i < bricksY; i++) {
		for (int j = 0; j < bricksX; j++) {
			modelMatrix = glm::mat3(1);

			if (logic.bricks.state[i][j] != 2) {
				modelMatrix *= Transform2D::Translate(logic.bricks.posX[i][j], logic.bricks.posY[i][j]);
				modelMatrix *= Transform2D::Scale(logic.bricks.scale[i][j], logic.bricks.scale[i][j]);
				RenderMesh2D(meshes["brick"], shaders["VertexColor"], modelMatrix);
			}

			if (logic.bricks.powerup[i][j] != 0 && logic.bricks.state[i][j] != 0 && logic.bricks.powerupY[i][j] >= 0) {
				modelMatrix = glm::mat3(1);
				modelMatrix *= Transform2D::Translate(logic.bricks.posX[i][j], logic.bricks.powerupY[i][j]);
				modelMatrix *= Transform2D::Rotate(logic.bricks.powerupAngle[i][j]);

				switch (logic.bricks.powerup[i][j])
				{
				case 1:
					RenderMesh2D(meshes["powerup"], shaders["VertexColor"], modelMatrix);
					break;

				case 2:
					RenderMesh2D(meshes["powerup2"], shaders["VertexColor"], modelMatrix);
					break;

				case 3:
					RenderMesh2D(meshes["powerup3"], shaders["VertexColor"], modelMatrix);
					break;

				default:
					break;
				}
			}

		}
	}

	
	
	

}

void Tema1::FrameEnd()
{

}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{

}

void Tema1::OnKeyPress(int key, int mods)
{
	// add key press event
}

void Tema1::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	platformX = mouseX;
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	glm::ivec2 resolution = window->GetResolution();

	if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {
		if (platformX > logic.borderWidth + logic.ballRadius && platformX + logic.ballRadius < resolution.x - logic.borderWidth) {
			logic.gameState = 1;
		}
	}
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}
