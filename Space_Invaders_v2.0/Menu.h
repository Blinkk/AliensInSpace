/************************************************
*  Programmer: Austin Ivicevic & Salvador Luna  *
*  SGPG215 Space Invaders						*
*  Date Created: 1/12/14						*
*************************************************/

#ifndef MENU_H
#define MENU_H
#include "DirectX.h"
#include "Sprite.h"
#include "Font.h"
#include <iostream>
#include <vector>
//using namespace std;

struct Button
{
	enum ButtonTypes { START = 0, CLOSE };
	LPDIRECT3DTEXTURE9 btn;
	Font* fontObj;
	int x, y;
	int width, height;
	int frame;
	unsigned int buttonType;		

	Button() {}

	// You must specify position using magic numbers as of right now
	// This button uses two separate images (one standard & one highlighted)
	Button(std::string filename,int X, int Y, int W, int H, 
		unsigned int btnType)
	{
		x = X;
		y = Y;
		width = W;
		height = H;
		buttonType = btnType;
		btn = LoadTexture(filename, D3DCOLOR_XRGB(255, 255, 255));

		fontObj = nullptr;
		fontObj = new Font("Calibri", 36);
	}

	~Button()
	{
		if (btn) btn->Release();
		if (fontObj)
		{
			delete fontObj;
			fontObj = nullptr;
		}
	}

	void Draw()
	{
		// Draw sprite
		Sprite_Draw_Frame(btn, x, y, frame, width, height, 1);

		// Draw text
		spriteobj->Begin(D3DXSPRITE_ALPHABLEND);
		int xPos, yPos;
		switch (buttonType)
		{
		case START:
			xPos = x + (width / 2) - (fontObj->getTextWidth("START") / 2);
			yPos = y + (height / 2) - (fontObj->getTextHeight("START") / 2);
			fontObj->Print(xPos, yPos, "START", D3DCOLOR_XRGB(255, 255, 255));
			break;

		case CLOSE:
			xPos = x + (width / 2) - (fontObj->getTextWidth("EXIT") / 2);
			yPos = y + (height / 2) - (fontObj->getTextHeight("EXIT") / 2);
			fontObj->Print(xPos, yPos, "EXIT", D3DCOLOR_XRGB(255, 255, 255));
			break;

		default:
			break;
		}
		spriteobj->End();
	}
};

class Menu
{
private:
	std::vector<Button*> buttons;
	LPDIRECT3DSURFACE9 background;
	double scrollY;
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