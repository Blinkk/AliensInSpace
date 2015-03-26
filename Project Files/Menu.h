/************************************************
*  Programmer: Austin Ivicevic & Salvador Luna  *
*  SGPG215 Space Invaders						*
*  Date Created: 1/12/14						*
*************************************************/

#ifndef MENU_H
#define MENU_H
#include "DirectX.h"
#include "Sprite.h"
#include <iostream>
#include <vector>
//using namespace std;

struct Button
{
	LPDIRECT3DTEXTURE9 btn;
	LPDIRECT3DTEXTURE9 btnS;
	LPDIRECT3DTEXTURE9 orig;
	int x, y;
	int width, height;

	Button() {}

	// You must specify position using magic numbers as of right now
	// This button uses two separate images (one standard & one highlighted)
	Button(std::string filename, std::string filename2,
		int X, int Y, int W, int H)
	{
		x = X;
		y = Y;
		width = W;
		height = H;
		btn = LoadTexture(filename, D3DCOLOR_XRGB(255, 255, 255));
		btnS = LoadTexture(filename2, D3DCOLOR_XRGB(255, 255, 255));
		orig = btn;
	}

	// Leaking sprites
	//virtual ~Button()
	//{
	//	// Deconstruct (delete sprites)
	//	if (btn)
	//	{
	//		delete btn;
	//		btn = NULL;
	//	}
	//	if (btnS)
	//	{
	//		delete btnS;
	//		btnS = NULL;
	//	}
	//	if (orig)
	//	{
	//		delete orig;
	//		orig = NULL;
	//	}
	//}

	void Draw()
	{
		Sprite_Draw_Frame(btn, x, y, 0, width, height, 1);
	}
};

class Menu
{
private:
	std::vector<Button*> buttons;
	LPDIRECT3DSURFACE9 background;
	HWND window;
	bool active;
	bool btnStatus[2];

public:
	Menu() { };
	Menu(std::string filename_bckgrnd, HWND wind);
	virtual ~Menu();
	void AddButton(Button *b);
	bool CheckButtonStatus(int b);
	void DrawMenu();
	void Deactivate();
	bool IsActive();
};

#endif