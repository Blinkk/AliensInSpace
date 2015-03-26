#include "Menu.h"

Menu::Menu(std::string filename_bckgrnd, HWND wind)
{
	background = LoadSurface(filename_bckgrnd);
	window = wind;
	active = true;
	for (int i = 0; i < 2; i++)
		btnStatus[i] = false;
}

Menu::~Menu()
{
	if (background) background->Release();
	if (buttons.size() > 0)
	{
		std::vector<Button*>::iterator i = buttons.begin();
		while (buttons.size() > 0)
		{
			buttons.erase(i);
			i = buttons.begin();
		}
	}
}

void Menu::AddButton(Button *b)
{
	buttons.push_back(b);
}

void Menu::DrawMenu()
{
	static std::vector<Button*>::iterator buttonIt;

	// Get mouse position
	static POINT mousePos;
	static int mouseX, mouseY;
	static int cntr = 1;
	GetCursorPos(&mousePos);
	ScreenToClient(window, &mousePos);
	mouseX = mousePos.x;
	mouseY = mousePos.y + 30;

	// Draw background
	static RECT sourceRect = { 0, 0, SCREENW, SCREENH };
	d3ddev->StretchRect(background, &sourceRect, backbuffer, NULL,
		D3DTEXF_NONE);

	for (buttonIt = buttons.begin(); buttonIt != buttons.end(); buttonIt++)
	{
		(*buttonIt)->Draw();

		// Check if mouse hovering over button
		// if a button is clicked, set menu to false
		if (mouseX >= (*buttonIt)->x && mouseX <= (*buttonIt)->x + (*buttonIt)->width
			&& mouseY >= (*buttonIt)->y && mouseY <= (*buttonIt)->y + (*buttonIt)->height)
		{
			(*buttonIt)->btn = (*buttonIt)->btnS;
			if (Mouse_Button(MB_DEFBUTTON1))
			{
				Sleep(500);
				active = false;
				btnStatus[cntr] = true;
			}		
		}
		else
			(*buttonIt)->btn = (*buttonIt)->orig;
		
		cntr++;
		if (cntr > buttons.size())
			cntr = 1;
	}

	
}

void Menu::Deactivate()
{
	active = false;
}

bool Menu::IsActive()
{
	return active;
}

bool Menu::CheckButtonStatus(int b)
{
	if (btnStatus[b] == true)
		return true;
	else
		return false;
}