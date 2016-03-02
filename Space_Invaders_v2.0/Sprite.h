/************************************************
*  Programmer: Austin Ivicevic & Salvador Luna  *
*  SGPG215 Space Invaders						*
*  Date Created: 1/12/14						*
*************************************************/

#ifndef SPRITE_H
#define SPRITE_H
#include "DirectX.h"

// Sprite class
class Sprite
{
protected:
	float _x, _y;
	int _frame, _columns;
	int _width, _height;
	float _scaling, _rotation;
	int _startframe, _endframe;
	int _starttime;
	int _delay;
	int _direction;
	float _velx, _vely;
	bool _active;
	D3DCOLOR _color;
	LPDIRECT3DTEXTURE9 _texture;

	// RECT for collision detection
	RECT* _hitBox;

public:
	Sprite();
	Sprite(LPDIRECT3DTEXTURE9 texture, int x, int y, int frame, int fWidth, 
		int fHeight,int velx, int vely, float scaling, int columns,
		float rotation,	int startframe = 0, int endframe = 0, 
		int direction = 1, int delay = 250, D3DCOLOR color = D3DCOLOR_XRGB(255,255,255),
		int starttime = 0);
	virtual ~Sprite();
	void Draw_Frame(int frm);
	void Draw_Current_Frame();
	void Sprite_Animate(int startframe, int endframe, int delay);
	void Sprite_Animate_Stop(int startframe, int endframe, int delay);
	void ChangeTexture(LPDIRECT3DTEXTURE9 txture);
	void Deactivate();
	void Activate();
	bool IsActive();

	void UpdateCollider();

	// Get _ Set functions
	int GetX();
	int GetY();
	void SetX(int x);
	void SetY(int y);
	int GetVelX();
	int GetVelY();
	void SetVelX(int velx);
	void SetVelY(int vely);
	int GetFrame();
	void SetFrame(int f);
	int GetWidth();
	int GetHeight();
	int GetStartFrame();
	int GetEndFrame();
	int GetDelay();
	int GetStartTime();
	void SetStartTime(int st);
	RECT GetHitbox();
};

#endif

