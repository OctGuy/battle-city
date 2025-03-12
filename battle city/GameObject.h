#pragma once

#include <Windows.h>
#include <d3dx10.h>

#include "Texture.h"
#include "Game.h"

class CGameObject
{
protected:
	float x;
	float y;

	// This should be a pointer to an object containing all graphic/sound/audio assets for rendering this object. 
	// For now, just a pointer to a single texture
	LPTEXTURE upTexture;
	LPTEXTURE downTexture;
	LPTEXTURE leftTexture;
	LPTEXTURE rightTexture;

public:
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	float GetX() { return x; }
	float GetY() { return y; }

	CGameObject(float x = 0.0f, float y = 0.0f, LPTEXTURE upTexture = NULL, LPTEXTURE downTexture = NULL, LPTEXTURE leftTexture = NULL, LPTEXTURE rightTexture = NULL);

	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0;

	~CGameObject();
};
typedef CGameObject* LPGAMEOBJECT;

class MoveableObject : public CGameObject {
protected:
	float vx;
	float vy;
public:
	MoveableObject(float x, float y, float vx, float vy,
		LPTEXTURE lTex, LPTEXTURE rTex, LPTEXTURE uTex, LPTEXTURE dTex) : CGameObject(x, y, lTex, rTex, uTex, dTex)
	{
		this->vx = vx;
		this->vy = vy;
	}
	void SetVelocity(float vx, float vy) { this->vx = vx, this->vy = vy; }
	float GetVx() { return vx; }
	float GetVy() { return vy; }
	void SetVx(float vx) { this->vx = vx; }
	void SetVy(float vy) { this->vy = vy; }
};

class Bullet : public MoveableObject {
protected:
	bool isActive;
public:
	Bullet(float x, float y, float vx, float vy, LPTEXTURE lTex) : MoveableObject(x, y, vx, vy, lTex, NULL, NULL, NULL)
	{
		this->isActive = false;
	}
	bool GetIsActive() { return isActive; }
	void Update(DWORD dt) override;
	void Render() override;
	void SetIsActive(bool isActive) { this->isActive = isActive; }
};

class Tank : public MoveableObject {
protected:
	Bullet* bullet;
	int direction;
	bool isMoving;
public:
	Tank(float x, float y, float vx, float vy,
		LPTEXTURE lTex, LPTEXTURE rTex, LPTEXTURE uTex, LPTEXTURE dTex,
		float bl_x, float bl_y, float bl_vx, float bl_vy,
		LPTEXTURE bl_lTex, LPTEXTURE bl_rTex, LPTEXTURE bl_uTex, LPTEXTURE bl_dTex)
		: MoveableObject(x, y, vx, vy, lTex, rTex, uTex, dTex)
	{
		this->bullet = new Bullet(bl_x, bl_y, bl_vx, bl_vy, bl_lTex);
		this->direction = 0;
		this->isMoving = false;
	};

	Bullet* GetBullet() { return bullet; }
	virtual void SetDirection() = 0;
	int GetDirection() { return direction; }
	virtual void HandleShooting(DWORD dt) = 0;
	void Render() override;
};

class Player : public Tank {
public:
	Player(float x, float y, float vx, float vy,
		LPTEXTURE lTex, LPTEXTURE rTex, LPTEXTURE uTex, LPTEXTURE dTex,
		float bl_x, float bl_y, float bl_vx, float bl_vy,
		LPTEXTURE bl_lTex, LPTEXTURE bl_rTex, LPTEXTURE bl_uTex, LPTEXTURE bl_dTex)
		: Tank(x, y, vx, vy, lTex, rTex, uTex, dTex, bl_x, bl_y, bl_vx, bl_vy, bl_lTex, bl_rTex, bl_uTex, bl_dTex) {
	};

	void Update(DWORD dt) override;
	void HandleShooting(DWORD dt) override;
	void SetDirection() override;
	//int GetDirection() override;
};

class Enemy : public Tank {
public:
	Enemy(float x, float y, float vx, float vy,
		LPTEXTURE lTex, LPTEXTURE rTex, LPTEXTURE uTex, LPTEXTURE dTex,
		float bl_x, float bl_y, float bl_vx, float bl_vy,
		LPTEXTURE bl_lTex, LPTEXTURE bl_rTex, LPTEXTURE bl_uTex, LPTEXTURE bl_dTex)
		: Tank(x, y, vx, vy, lTex, rTex, uTex, dTex, bl_x, bl_y, bl_vx, bl_vy, bl_lTex, bl_rTex, bl_uTex, bl_dTex) {
	};

	//void Update(DWORD dt) override;
	//void HandleShooting(DWORD dt) override;
	//int GetDirection() override;
};

