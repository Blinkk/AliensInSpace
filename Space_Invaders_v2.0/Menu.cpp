#include "Menu.h"

Menu::Menu(std::string filename_bckgrnd, HWND wind)
{
	LPDIRECT3DSURFACE9 image = LoadSurface(filename_bckgrnd);
	if (!image) return;

	// Create the background 
	HRESULT result = d3ddev->CreateOffscreenPlainSurface(
		800, 1200, D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT, &background,
		NULL);
	if (result != D3D_OK) return;

	// Copy the image to the upper left corner of the background
	RECT source_rect = { 0, 0, SCREENW, SCREENH };
	RECT dest_ul = { 0, 0, SCREENW, SCREENH };
	d3ddev->StretchRect(image, &source_rect, background, &dest_ul, D3DTEXF_NONE);

	//copy the image into the lower left corner of the background
	RECT dest_ll = { 0, 600, SCREENW, SCREENH * 2 };
	d3ddev->StretchRect(image, &source_rect, background, &dest_ll, D3DTEXF_NONE);

	// pointer to the backbuffer
	d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
	image->Release();

	scrollY = 0;


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

	// Scroll the background
	scrollY--;
	if (scrollY < 0) scrollY = 1200 - SCREENH - 1;
	if (scrollY > 1200 - SCREENH - 1) scrollY = 0;

	// Draw background
	RECT source_rect = { 0, scrollY, SCREENW, scrollY + SCREENH };
	RECT dest_rect = { 0, 0, SCREENW, SCREENH };
	d3ddev->StretchRect(background, &source_rect, backbuffer, &dest_rect, D3DTEXF_NONE);

	for (buttonIt = buttons.begin(); buttonIt != buttons.end(); buttonIt++)
	{
		(*buttonIt)->Draw();

		// Check if mouse hovering over button
		// if a button is clicked, set menu to false
		if (mouseX >= (*buttonIt)->x && mouseX <= (*buttonIt)->x + (*buttonIt)->width
			&& mouseY >= (*buttonIt)->y && mouseY <= (*buttonIt)->y + (*buttonIt)->height)
		{
			(*buttonIt)->frame = 1;
			if (Mouse_Button(MB_DEFBUTTON1))
			{
				Sleep(500);
				active = false;
				btnStatus[cntr] = true;
			}
		}
		else
			(*buttonIt)->frame = 0;
		
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