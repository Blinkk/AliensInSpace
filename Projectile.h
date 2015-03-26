/********************************
*  Programmer: Austin Ivicevic  *
*  2_Hour_game					*
*  Date Created: 12/1/14        *
*********************************/
#include "DirectX.h"
#include "Sprite.h"

class Projectile : public Sprite
{
public:
	bool Active;

	void MoveUp();
	void MoveDown();
	void SetX(int X);
	void SetY(int Y);
	Projectile();
	Projectile(LPDIRECT3DTEXTURE9 texture, int x, int y, int frame, int fWidth,
		int fHeight, int velx, int vely, float scaling, int columns,
		float rotation, int startframe = 0, int endframe = 0,
		int direction = 1, int delay = 250, D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255));
	bool IsActive();
	void Deactivate();
	bool deleteMe;
	~Projectile();
};