/************************************************
*  Programmer: Austin Ivicevic & Salvador Luna  *
*  SGPG215 Space Invaders						*
*  Date Created: 1/12/14						*
*************************************************/

#include "DirectX.h"
#include <iostream>
#include "Enemy.h"

Enemy::Enemy(LPDIRECT3DTEXTURE9 texture, int x, int y, int frame,
	int fWidth, int fHeight, int velx, int vely, float scaling, int columns,
	float rotation, int startframe, int endframe, int direction, int delay, D3DCOLOR color)
	: Sprite(texture, x, y, frame, fWidth, fHeight, velx, vely, scaling, columns, 
	rotation, startframe, endframe, direction, delay, color)
{
	_active = true;
}

Enemy::~Enemy()
{
	
}

bool Enemy::IsActive()
{
	return _active;
}

void Enemy::Deactivate()
{
	_active = false;
}