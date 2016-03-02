/************************************************
*  Programmer: Austin Ivicevic & Salvador Luna  *
*  SGPG215 Space Invaders						*
*  Date Created: 1/12/14						*
*************************************************/

#pragma warning(disable : 4244)
#include "Sprite.h"
#include "DirectX.h"
#include <iostream>
//using namespace std;

// Default constructor
Sprite::Sprite()
{
	_frame = 0;
	_columns = 1;
	_width = _height = 0;
	_scaling = 1.0f;
	_rotation = 0.0f;
	_startframe = _endframe = 0;
	_direction = 1;
	_starttime = _delay = 0;
	_velx = _vely = 0.0f;
	_color = D3DCOLOR_XRGB(255, 255, 255);
	_texture = NULL;
	_active = true;

	_hitBox = nullptr;
}

// Constructor
Sprite::Sprite(LPDIRECT3DTEXTURE9 texture, int x, int y, int frame,
	int fWidth, int fHeight,int velx, int vely, float scaling, int columns, 
	float rotation,	int startframe, int endframe, int direction, int delay, D3DCOLOR color,
	int starttime)
{
	_x = x; _y = y;
	_texture = texture;
	_frame = frame;
	_width = fWidth;
	_height = fHeight;
	_velx = velx;
	_vely = vely;
	_scaling = scaling;
	_columns = columns;
	_rotation = rotation;
	_startframe = startframe;
	_endframe = endframe;
	_direction = direction;
	_delay = delay;
	_color = color;
	_starttime = starttime;
	_active = true;

	_hitBox = new RECT();
	_hitBox->left = _x;
	_hitBox->top = _y;
	_hitBox->right = _hitBox->left + _width;
	_hitBox->bottom = _hitBox->top + _height;
}

// Destructor (virtual)
Sprite::~Sprite()
{
	if (_texture != NULL)
		_texture->Release();
}

/* Draw a specific frame from texture */
void Sprite::Draw_Frame(int frm)
{
	D3DXVECTOR3 position((float)_x, (float)_y, 0);
	D3DCOLOR white = D3DCOLOR_XRGB(255, 255, 255);

	RECT rect;
	rect.left = (frm % _columns) * _width;
	rect.top = (frm / _columns) * _height;
	rect.right = rect.left + _width;
	rect.bottom = rect.top + _height;

	spriteobj->Begin(D3DXSPRITE_ALPHABLEND);	// Begin spritebatch
	spriteobj->Draw(_texture, &rect, NULL, &position, _color);
	spriteobj->End();							// End spritebatch
}

/* Draw a specific frame from texture */
void Sprite::Draw_Current_Frame()
{
	D3DXVECTOR3 position((float)_x, (float)_y, 0);
	D3DCOLOR white = D3DCOLOR_XRGB(255, 255, 255);

	RECT rect;
	rect.left = (_frame % _columns) * _width;
	rect.top = (_frame / _columns) * _height;
	rect.right = rect.left + _width;
	rect.bottom = rect.top + _height;
	
	spriteobj->Begin(D3DXSPRITE_ALPHABLEND);	// Begin spritebatch
	spriteobj->Draw(_texture, &rect, NULL, &position, _color);
	spriteobj->End();							// End spritebatch
}

/* Animate texture (sprite sheet) */
void Sprite::Sprite_Animate(int startframe, 
	int endframe, int delay)
{
	if ((int)GetTickCount() > _starttime + delay)
	{
		_starttime = GetTickCount();
		_frame += _direction;
		if (_frame > endframe) _frame = startframe;
		if (_frame < startframe) _frame = endframe;
	}
}

void Sprite::Sprite_Animate_Stop(int startframe, int endframe, int delay)
{		
	if ((int)GetTickCount() > _starttime + delay)
	{
		// If it reached the end of animation, stop
		if (_frame == endframe)
		{
			_frame = endframe;
			return;
		}

		_starttime = GetTickCount();
		_frame += _direction;
		if (_frame < startframe) _frame = startframe;
	}
}

void Sprite::ChangeTexture(LPDIRECT3DTEXTURE9 txture)
{
	if (txture != NULL)
		_texture = txture;
}

void Sprite::UpdateCollider()
{
	if (_hitBox)
	{
		_hitBox->left = _x;
		_hitBox->top = _y;
		_hitBox->right = _hitBox->left + _width;
		_hitBox->bottom = _hitBox->top + _height;
	}
}

// Get functions
int Sprite::GetX()
{
	return _x;
}

int Sprite::GetY()
{
	return _y;
}

void Sprite::SetX(int x)
{
	_x = x;
	this->UpdateCollider();
}

void Sprite::SetY(int y)
{
	_y = y;
	this->UpdateCollider();
}

int Sprite::GetVelX()
{
	return _velx;
}

int Sprite::GetVelY()
{
	return _vely;
}

void Sprite::SetVelX(int velx)
{
	_velx = velx;
}

void Sprite::SetVelY(int vely)
{
	_vely = vely;
}

int Sprite::GetFrame()
{
	return _frame;
}

void Sprite::SetFrame(int f)
{
	_frame = f;
}

int Sprite::GetWidth()
{
	return _width;
}

int Sprite::GetHeight()
{
	return _height;
}

int Sprite::GetStartFrame()
{
	return _startframe;
}

int Sprite::GetEndFrame()
{
	return _endframe;
}

int Sprite::GetDelay()
{
	return _delay;
}

int Sprite::GetStartTime()
{
	return _starttime;
}

void Sprite::SetStartTime(int st)
{
	_starttime = st;
}

void Sprite::Deactivate()
{
	_active = false;
}

void Sprite::Activate()
{
	_active = true;
}

bool Sprite::IsActive()
{
	return _active;
}

RECT Sprite::GetHitbox()
{
	if (_hitBox)
		return (*_hitBox);
	else
	{
		RECT* defaultRect = new RECT();
		defaultRect->left = 0;
		defaultRect->top = 0;
		defaultRect->right = 0;
		defaultRect->bottom = 0;

		return (*defaultRect);
	}
}