#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include <cstdlib>
#include <ctime>

/*
	Initialize game object
*/

#define BULLET_SPEED 0.2f

CGameObject::CGameObject(float x, float y, LPTEXTURE lTex, LPTEXTURE rTex, LPTEXTURE uTex, LPTEXTURE dTex)
{
	this->x = x;
	this->y = y;
	this->leftTexture = lTex;
	this->rightTexture = rTex;
	this->upTexture = uTex;
	this->downTexture = dTex;
}

//void CGameObject::Render()
//{
//	//CGame::GetInstance()->Draw(x, y, texture);
//}

CGameObject::~CGameObject()
{
	if (leftTexture != NULL) delete leftTexture;
	if (downTexture != NULL) delete downTexture;
	if (rightTexture != NULL) delete rightTexture;
	if (upTexture != NULL) delete upTexture;
}

void Bullet::Update(DWORD dt) {
	if (isActive) {
		/*Player* player = CGame::GetInstance()->GetPlayer();
		int bulletDirection = player->GetDirection();*/

		if (direction == 1) {
			vy = -BULLET_SPEED;
			vx = 0;
		}
		else if (direction == 2) {
			vy = BULLET_SPEED;
			vx = 0;
		}
		else if (direction == 3) {
			vx = -BULLET_SPEED;
			vy = 0;
		}
		else if (direction == 4) {
			vx = BULLET_SPEED;
			vy = 0;
		}

		x += vx * dt;
		y += vy * dt;
	}
}

void Bullet::Render() {
	if (isActive) CGame::GetInstance()->Draw(x, y, leftTexture);
}

//void Tank::Update(DWORD dt) {
//
//}

//void Tank::HandleShooting(DWORD dt) {
//
//}

void Tank::Render() {
	CGame::GetInstance()->Draw(x, y, upTexture);

	if (direction == 1) {
		CGame::GetInstance()->Draw(x, y, upTexture);
	}
	else if (direction == 2) {
		CGame::GetInstance()->Draw(x, y, downTexture);
	}
	else if (direction == 3) {
		CGame::GetInstance()->Draw(x, y, leftTexture);
	}
	else if (direction == 4) {
		CGame::GetInstance()->Draw(x, y, rightTexture);
	}

	bullet->Render();
}

#define PLAYER_SPEED 0.2f
#define PLAYER_WIDTH 20
#define PLAYER_HEIGHT 20

void Player::SetDirection() {
	if (GetAsyncKeyState('W') & 0x8000) {
		direction = 1;
		isMoving = true;
	}
	else if (GetAsyncKeyState('S') & 0x8000) {
		direction = 2;
		isMoving = true;
	}
	else if (GetAsyncKeyState('A') & 0x8000) {
		direction = 3;
		isMoving = true;
	}
	else if (GetAsyncKeyState('D') & 0x8000) {
		direction = 4;
		isMoving = true;
	}
	else {
		isMoving = false;
	}
}

void Player::HandleShooting(DWORD dt) {
	if (!bullet->GetIsActive()) {
		bullet->SetIsActive(true);
		bullet->SetPosition(x, y);
		bullet->SetDirection(direction);

		/*if (direction == 1) {
			bullet->SetVelocity
		}*/
		//bullet->SetVelocity(vx, vy);
	}
}

void Player::Update(DWORD dt)
{
	//CTank::Update(dt);
	// Reset velocity
	vx = 0;
	vy = 0;
	
	SetDirection();

	int newDirection = GetDirection();
	if (newDirection != 0) {
		direction = newDirection;
	}

	// Check for key presses and update velocity accordingly
	if (GetDirection() == 1 && isMoving) {
		vy = -PLAYER_SPEED;  // Move up
	}
	if (GetDirection() == 2 && isMoving) {
		vy = PLAYER_SPEED;   // Move down
	}
	if (GetDirection() == 3 && isMoving) {
		vx = -PLAYER_SPEED;  // Move left
	}
	if (GetDirection() == 4 && isMoving) {
		vx = PLAYER_SPEED;   // Move right
	}
	// Update spaceship's position based on velocity    
	x += vx * dt;
	y += vy * dt;
	// Get the back buffer width and height for boundary checking
	int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
	int BackBufferHeight = CGame::GetInstance()->GetBackBufferHeight();
	// Boundary checking for x-axis
	if (x <= PLAYER_WIDTH / 2) {
		x = PLAYER_WIDTH / 2;
	}
	else if (x >= BackBufferWidth - PLAYER_WIDTH / 2) {
		x = (float)(BackBufferWidth - PLAYER_WIDTH / 2);
	}
	// Boundary checking for y-axis
	if (y <= PLAYER_HEIGHT / 2) {
		y = PLAYER_HEIGHT / 2;
	}
	else if (y >= BackBufferHeight - PLAYER_HEIGHT / 2) {
		y = (float)(BackBufferHeight - PLAYER_HEIGHT / 2);
	}

	// Handle Shooting 
	if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
		HandleShooting(dt);
	}

	if (bullet->GetIsActive()) {
		if (bullet->GetX() < 0 || bullet->GetX() > BackBufferWidth
			|| bullet->GetY() < 0 || bullet->GetY() > BackBufferHeight)
			bullet->SetIsActive(false);
	}

	bullet->Update(dt);
}