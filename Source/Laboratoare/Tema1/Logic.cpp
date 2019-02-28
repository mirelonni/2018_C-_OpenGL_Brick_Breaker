#include "Logic.h"

#include <Core/Engine.h>

Logic::Logic()
{
	
}

Logic::Logic(int bricksX, int bricksY)
{
	this->bricksX = bricksX;
	this->bricksY = bricksY;
	lifes = 0;


	int chance;
	bricks.state.resize(bricksY);
	bricks.posX.resize(bricksY);
	bricks.posY.resize(bricksY);
	bricks.scale.resize(bricksY);
	bricks.powerup.resize(bricksY);
	bricks.powerupY.resize(bricksY);
	bricks.powerupAngle.resize(bricksY);

	for (int i = 0; i < bricksY; i++) {

		bricks.state[i].resize(bricksX);
		bricks.posX[i].resize(bricksX);
		bricks.posY[i].resize(bricksX);
		bricks.scale[i].resize(bricksX);
		bricks.powerup[i].resize(bricksX);
		bricks.powerupY[i].resize(bricksX);
		bricks.powerupAngle[i].resize(bricksX);
		
	}
	
	speedX = 0;
	speedY = 1;
	
}

Logic::~Logic()
{

}

void Logic::Loop(float platformX, float deltaTimeSeconds)
{

	float ballNextX, ballNextY; // aux for next frame ball position
	ballNextX = ballX + ballSpeedX * deltaTimeSeconds;
	ballNextY = ballY + ballSpeedY * deltaTimeSeconds;

	float bSX, bSY; // aux for calculating the new ball speed

	
	if (powerup1Time > 0) {
		powerup1Time -= powerup1Speed * deltaTimeSeconds;
	}
	else {
		powerup1Time = 0.0f;
	}

	if (powerup2Time > 0) {
		powerup2Time -= powerup2Speed * deltaTimeSeconds;
	}
	else {
		powerup2Time = 0.0f;
	}

	if (lifes == 0) {

		// reset the bricks
		
		lifes = initialLifes;
		for (int i = 0; i < bricksY; i++) {
			for (int j = 0; j < bricksX; j++) {
				bricks.state[i][j] = 0;
				bricks.scale[i][j] = 1;
				bricks.powerupY[i][j] = bricks.posY[i][j];
				bricks.powerupAngle[i][j] = 0;
				int chance = rand() % 10; // 10% chance for a powerup

				if (chance <= 3) {
					bricks.powerup[i][j] = chance;
				}
			}
		}
	}

	if (gameState == 1) {
		if (((ballX + ballRadius) <= (windowX - borderWidth) &&
			(ballNextX + ballRadius) >= (windowX - borderWidth)) ||
			(ballX + ballRadius) >= (windowX - borderWidth)) {
			// right border
			speedX = -1;
			speedY = 1;
			ballX = ballX + speedX * ballSpeedX * deltaTimeSeconds;
			ballY = ballY + speedY * ballSpeedY * deltaTimeSeconds;
			ballSpeedX = speedX * ballSpeedX;
			ballSpeedY = speedY * ballSpeedY;
			return;
		}

		if (((ballX - ballRadius) >= (borderWidth) &&
			(ballNextX - ballRadius) <= (borderWidth)) ||
			(ballX - ballRadius) <= (borderWidth)) {
			// left border
			speedX = -1;
			speedY = 1;
			ballX = ballX + speedX * ballSpeedX * deltaTimeSeconds;
			ballY = ballY + speedY * ballSpeedY * deltaTimeSeconds;
			ballSpeedX = speedX * ballSpeedX;
			ballSpeedY = speedY * ballSpeedY;
			return;
		}

		if ((ballY + ballRadius) <= (windowY - borderWidth) &&
			(ballNextY + ballRadius) >= (windowY - borderWidth)) {
			// top border
			speedX = 1;
			speedY = -1;
			ballX = ballX + speedX * ballSpeedX * deltaTimeSeconds;
			ballY = ballY + speedY * ballSpeedY * deltaTimeSeconds;
			ballSpeedX = speedX * ballSpeedX;
			ballSpeedY = speedY * ballSpeedY;
			return;
		}

		if ((ballY - ballRadius) >= (borderWidth / 2) &&
			(ballNextY - ballRadius) <= (borderWidth / 2)
			&& powerup1Time > 0) {
			// top border
			speedX = 1;
			speedY = -1;
			ballX = ballX + speedX * ballSpeedX * deltaTimeSeconds;
			ballY = ballY + speedY * ballSpeedY * deltaTimeSeconds;
			ballSpeedX = speedX * ballSpeedX;
			ballSpeedY = speedY * ballSpeedY;
			return;
		}






		if ((ballY - ballRadius) >= (2 * platformHeight) &&
			(ballNextY - ballRadius) <= (2 * platformHeight)) {
			// platform colision
			if (ballX >= platformX - platformWidth / 2 - ballRadius &&
				ballX <= platformX + platformWidth / 2 + ballRadius) {

				if (ballX == platformX) {
					// center of the platform
					speedX = 1;
					speedY = -1;
					ballX = ballX + speedX * ballSpeedX * deltaTimeSeconds;
					ballY = ballY + speedY * ballSpeedY * deltaTimeSeconds;
					ballSpeedX = speedX * ballSpeedX;
					ballSpeedY = speedY * ballSpeedY;
					return;
				}

				if (ballX < platformX) {
					// left side of the platform
					float arccos = acos((platformX - (ballX)) / (platformWidth / 2 + ballRadius));
					bSX = -ballSpeed * cos(arccos);
					bSY = ballSpeed * sin(arccos);
					ballX = ballX + bSX * deltaTimeSeconds;
					ballY = ballY + bSY * deltaTimeSeconds;
					ballSpeedX = bSX;
					ballSpeedY = bSY;
					return;
				}

				if (ballX > platformX) {
					// right side of the platform
					float arccos = acos(((ballX)-platformX) / (platformWidth / 2 + ballRadius));
					bSX = ballSpeed * cos(arccos);
					bSY = ballSpeed * sin(arccos);
					ballX = ballX + bSX * deltaTimeSeconds;
					ballY = ballY + bSY * deltaTimeSeconds;
					ballSpeedX = bSX;
					ballSpeedY = bSY;
					return;
				}

			}

		}
	}

	if ((ballY - ballRadius) <= 0) {
		// ball droped
		lifes--;
		gameState = 0;
		speedX = 0;
		speedY = 1;

		ballX = platformX;
		ballY = 2 * platformHeight;
		ballSpeedX = 0;
		ballSpeedY = ballSpeed;
		return;
	}


	for (int i = 0; i < bricksY; i++) {
		for (int j = 0; j < bricksX; j++) {

			if (gameState == 1) {

				// bricks colision
				if (bricks.state[i][j] == 0) {
					
					if (ballY + ballRadius <= bricks.posY[i][j] - brickHeight / 2 &&
						ballNextY + ballRadius >= bricks.posY[i][j] - brickHeight / 2 &&
						ballX + ballRadius >= bricks.posX[i][j] - brickWidth / 2 &&
						ballX - ballRadius <= bricks.posX[i][j] + brickWidth / 2) {
						// bottom of the brick

						if (!powerup2Time > 0) {
							speedX = 1;
							speedY = -1;
							ballX = ballX + speedX * ballSpeedX * deltaTimeSeconds;
							ballY = ballY + speedY * ballSpeedY * deltaTimeSeconds;
							ballSpeedX = speedX * ballSpeedX;
							ballSpeedY = speedY * ballSpeedY;
							bricks.state[i][j] = 1;
							return;
						}
						bricks.state[i][j] = 1;
						
						
					}
					if (ballY - ballRadius >= bricks.posY[i][j] + brickHeight / 2 &&
						ballNextY - ballRadius <= bricks.posY[i][j] + brickHeight / 2 &&
						ballX + ballRadius >= bricks.posX[i][j] - brickWidth / 2 &&
						ballX - ballRadius <= bricks.posX[i][j] + brickWidth / 2) {
						// top of the brick
						if (!powerup2Time > 0) {
							speedX = 1;
							speedY = -1;
							ballX = ballX + speedX * ballSpeedX * deltaTimeSeconds;
							ballY = ballY + speedY * ballSpeedY * deltaTimeSeconds;
							ballSpeedX = speedX * ballSpeedX;
							ballSpeedY = speedY * ballSpeedY;
							bricks.state[i][j] = 1;
							return;
						}
						bricks.state[i][j] = 1;
						
						
					}
					if (ballX + ballRadius <= bricks.posX[i][j] - brickWidth / 2 &&
						ballNextX + ballRadius >= bricks.posX[i][j] - brickWidth / 2 &&
						ballY + ballRadius >= bricks.posY[i][j] - brickHeight / 2 &&
						ballY - ballRadius <= bricks.posY[i][j] + brickHeight / 2) {
						// left of the brick
						if (!powerup2Time > 0) {
							speedX = -1;
							speedY = 1;
							ballX = ballX + speedX * ballSpeedX * deltaTimeSeconds;
							ballY = ballY + speedY * ballSpeedY * deltaTimeSeconds;
							ballSpeedX = speedX * ballSpeedX;
							ballSpeedY = speedY * ballSpeedY;
							bricks.state[i][j] = 1;
							return;
						}
						bricks.state[i][j] = 1;
						
						
					}
					if (ballX - ballRadius >= bricks.posX[i][j] + brickWidth / 2 &&
						ballNextX - ballRadius <= bricks.posX[i][j] + brickWidth / 2 &&
						ballY + ballRadius >= bricks.posY[i][j] - brickHeight / 2 &&
						ballY - ballRadius <= bricks.posY[i][j] + brickHeight / 2) {
						// right of the brick
						if (!powerup2Time > 0) {
							speedX = -1;
							speedY = 1;
							ballX = ballX + speedX * ballSpeedX * deltaTimeSeconds;
							ballY = ballY + speedY * ballSpeedY * deltaTimeSeconds;
							ballSpeedX = speedX * ballSpeedX;
							ballSpeedY = speedY * ballSpeedY;
							bricks.state[i][j] = 1;
							return;
						}
						bricks.state[i][j] = 1;
						
						
					}
				}
			}

			



			// scale the bricks
			if (bricks.state[i][j] == 1) {
				if (bricks.scale[i][j] > 0) {
					bricks.scale[i][j] -= brickScaleSpeed * deltaTimeSeconds;
				}
				else {
					bricks.scale[i][j] = 0;
					bricks.state[i][j] = 2;
				}
				
			}
			


			// powerup update
			if (bricks.state[i][j] != 0) {
				if (bricks.powerup[i][j] != 0) {
					bricks.powerupY[i][j] -= powerupDropSpeed * deltaTimeSeconds;
					bricks.powerupAngle[i][j] += powerupSpinSpeed * deltaTimeSeconds;
				}
			}



			// powerup pickup
			if (bricks.powerupY[i][j] <= 2 * platformHeight &&
				bricks.powerupY[i][j] >= 0) {
				if (bricks.posX[i][j] >= platformX - platformWidth / 2 &&
					bricks.posX[i][j] <= platformX + platformWidth / 2) {
					// use the powerup
					switch (bricks.powerup[i][j])
					{
					case 1:
						powerup1Time = powerup1TimeMax;
						break;

					case 2:
						if (lifes < maxLifes) {
							lifes++;
						}
						break;
					case 3:
						powerup2Time = powerup2TimeMax;
						break;

					default:
						break;
					}
					
					bricks.powerup[i][j] = 0;

				}
			}

		}
	}

	// normal condition
	if (gameState == 1) {
		ballX = ballNextX;
		ballY = ballNextY;
	}
	

	
	return;
}