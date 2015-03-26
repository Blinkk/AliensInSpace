/************************************************
*  Programmer: Austin Ivicevic & Salvador Luna  *
*  SGPG215 Space Invaders						*
*  Date Created: 1/12/14						*
*************************************************/

#ifndef ENEMY_H
#define ENEMY_H
#include "Sprite.h"
#include <iostream>

class Enemy : public Sprite
{
private:
	bool _active;

public:
	Enemy();
	Enemy(LPDIRECT3DTEXTURE9 texture, int x, int y, int frame,
		int fWidth, int fHeight, int velx, int vely, float scaling, int columns,
		float rotation, int startframe, int endframe, int direction, int delay,
		D3DCOLOR color = D3DCOLOR_XRGB(255,255,255));
	~Enemy();
	bool IsActive();
	void Deactivate();
};

#endif