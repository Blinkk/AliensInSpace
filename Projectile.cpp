/************************************************
*  Programmer: Austin Ivicevic & Salvador Luna  *
*  SGPG215 Space Invaders						*
*  Date Created: 1/12/14						*
*************************************************/
#pragma warning(disable : 4244)
#include "Projectile.h"

Projectile::Projectile()
{

}

void Projectile::MoveUp()
{
	if (_y > 0)
	{
		_y -= _vely;
		//Sprite_Animate(_startframe, _endframe, _delay);
	}
	else
		Deactivate();
}
void Projectile::MoveDown()
{
	if (_y < SCREENH)
	{
		_y += _vely;
		//Sprite_Animate(_startframe, _endframe, _delay);
	}
	else
		Deactivate();
}
void Projectile::SetX(int X)
{
	_x = X;
}
void Projectile::SetY(int Y)
{
	_y = Y;
}

Projectile::Projectile(LPDIRECT3DTEXTURE9 texture, int x, int y, int frame, int fWidth,
	int fHeight, int velx, int vely, float scaling, int columns,
	float rotation, int startframe, int endframe,
	int direction, int delay, D3DCOLOR color)
	: Sprite(texture, x, y, frame, fWidth, fHeight, velx, vely, scaling, columns, rotation,
	startframe, endframe, direction, delay, color)
{
	Active = true;
	deleteMe = false;
}

Projectile::~Projectile()
{

}

bool Projectile::IsActive()
{
	return Active;
}
void Projectile::Deactivate()
{
	Active = false;
}