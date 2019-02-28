#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

class Logic
{
public:
	Logic();
	Logic(int bricksX, int bricksY);
	~Logic();
	

	void Loop(float platformX, float deltaTimeSeconds);
	
	int lifes, gameState, maxLifes, initialLifes;

	int bricksX, bricksY;
	float brickWidth, brickHeight, brickScaleSpeed;

	float windowX, windowY, borderWidth, platformWidth, platformHeight;
	
	float ballRadius, ballSpeed, ballSpeedX, ballSpeedY, ballX, ballY;
	signed short speedX, speedY;

	float powerupDropSpeed, powerupSpinSpeed;
	float powerup1Time, powerup1TimeMax, powerup1Speed;
	float powerup2Time, powerup2TimeMax, powerup2Speed;

	struct brick
	{
		std::vector<std::vector<unsigned short>> state;

		std::vector<std::vector<float>> posX;
		std::vector<std::vector<float>> posY;
		std::vector<std::vector<float>> scale;
		
		std::vector<std::vector<unsigned short>> powerup;
		std::vector<std::vector<float>> powerupY;
		std::vector<std::vector<float>> powerupAngle;

	} bricks;
};