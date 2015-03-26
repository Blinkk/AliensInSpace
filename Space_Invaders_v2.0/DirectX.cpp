/************************************************
*  Programmer: Austin Ivicevic & Salvador Luna  *
*  SGPG215 Space Invaders						*
*  Date Created: 1/12/14						*
*************************************************/

/* DirectX Function Definitions */

#include "DirectX.h"
#include "Sprite.h"
#include <iostream>
//using namespace std;

// Direct3D variables
LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3ddev = NULL;
LPDIRECT3DSURFACE9 backbuffer = NULL;

// DirectInput variables
LPDIRECTINPUT8 dinput = NULL;
LPDIRECTINPUTDEVICE8 dimouse = NULL;
LPDIRECTINPUTDEVICE8 dikeyboard = NULL;
DIMOUSESTATE mouse_state;
XINPUT_GAMEPAD controllers[4];
LPD3DXSPRITE spriteobj = NULL;
char keys[256];

/* Direct3D initialization */
bool Direct3D_Init(HWND window, int width, int height, bool fullscreen)
{
	// Initialize Direct3D
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (!d3d) return false;

	// Set Direct3D presentation parameters
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = (!fullscreen);
	d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = width;
	d3dpp.BackBufferHeight = height;
	d3dpp.hDeviceWindow = window;

	// Create Direct3D device
	d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3ddev);

	if (!d3ddev) return false;

	// Get a pointer to the back buffer surface
	d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
	
	// Create sprite object
	D3DXCreateSprite(d3ddev, &spriteobj);

	return true;
}

/* Direct3D shutdown */
void Direct3D_Shutdown()
{
	if (d3ddev) d3ddev->Release();
	if (d3d) d3d->Release();
}

/* Draws a surface to the screen using StretchRect */
void DrawSurface(LPDIRECT3DSURFACE9 dest, float x, float y, LPDIRECT3DSURFACE9 source)
{
	// Get width/height from source surface
	D3DSURFACE_DESC desc;
	source->GetDesc(&desc);

	// Create rects for drawing
	RECT source_rect = { 0, 0, (long)desc.Width, (long)desc.Height };
	RECT dest_rect = { (long)x, (long)y, (long)x + desc.Width, (long)y + desc.Height };

	// Draw the source surface onto the dest
	d3ddev->StretchRect(source, &source_rect, dest, &dest_rect, D3DTEXF_NONE);

}

/* Loads a bitmap file into a surface */
LPDIRECT3DSURFACE9 LoadSurface(std::string filename)
{
	filename = "./Resources/" + filename;
	LPDIRECT3DSURFACE9 image = NULL;

	// Get width and height from bitmap file
	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFile(filename.c_str(), &info);
	if (result != D3D_OK)
		return NULL;

	// Create surface
	result = d3ddev->CreateOffscreenPlainSurface(
		info.Width,				// Width of the surface
		info.Height,			// Height of the surface
		D3DFMT_X8R8G8B8,		// Surface format
		D3DPOOL_DEFAULT,		// Memory pool to use
		&image,					// Pointer to the surface
		NULL);					// Reserved (always NULL)

	if (result != D3D_OK) return NULL;

	// Load surface from file into newly created surface
	result = D3DXLoadSurfaceFromFile(
		image,                  // Destination surface
		NULL,                   // Destination palette
		NULL,                   // Destination rectangle
		filename.c_str(),       // Source filename
		NULL,                   // Source rectangle
		D3DX_DEFAULT,           // Controls how image is filtered
		D3DCOLOR_XRGB(0, 0, 0), // For transparency (0 for none)
		NULL);                  // Source image info (usually NULL)

	// Make sure file was loaded okay
	if (result != D3D_OK) return NULL;

	return image;
}

/* Get bitmap width and height */
D3DXVECTOR2 GetBitmapSize(std::string filename)
{
	D3DXIMAGE_INFO info;
	D3DXVECTOR2 size = D3DXVECTOR2(0.0f, 0.0f);

	HRESULT result = D3DXGetImageInfoFromFile(filename.c_str(), &info);

	if (result == D3D_OK)
		size = D3DXVECTOR2((float)info.Width, (float)info.Height);

	return size;
}

/* Loads a bitmap file into a texture */
LPDIRECT3DTEXTURE9 LoadTexture(std::string filename, D3DCOLOR transcolor)
{
	filename = "./Resources/" + filename;
	LPDIRECT3DTEXTURE9 texture = NULL;

	// Get width and height from bitmap file
	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFile(filename.c_str(), &info);
	if (result != D3D_OK) return NULL;

	// Create the new texture by loading a bitmap image file
	D3DXCreateTextureFromFileEx(
		d3ddev,                // Direct3D device object
		filename.c_str(),      // Bitmap filename
		info.Width,            // Bitmap image width
		info.Height,           // Bitmap image height
		1,                     // Mip-map levels (1 for no chain)
		D3DPOOL_DEFAULT,       // The type of surface (standard)
		D3DFMT_UNKNOWN,        // Surface format (default)
		D3DPOOL_DEFAULT,       // Memory class for the texture
		D3DX_DEFAULT,          // Image filter
		D3DX_DEFAULT,          // Mip filter
		transcolor,            // Color key for transparency
		&info,                 // Bitmap file info (from loaded file)
		NULL,                  // Color palette
		&texture);             // Destination texture

	// Make sure the bitmap textre was loaded correctly
	if (result != D3D_OK) return NULL;

	return texture;
}

/* Draw a specific frame from texture */
void Sprite_Draw_Frame(LPDIRECT3DTEXTURE9 texture, int destx, int desty, int framenum, int framew, int frameh, int columns)
{
	D3DXVECTOR3 position((float)destx, (float)desty, 0);
	D3DCOLOR white = D3DCOLOR_XRGB(255, 255, 255);

	RECT rect;
	rect.left = (framenum % columns) * framew;
	rect.top = (framenum / columns) * frameh;
	rect.right = rect.left + framew;
	rect.bottom = rect.top + frameh;

	spriteobj->Begin(D3DXSPRITE_ALPHABLEND);
	spriteobj->Draw(texture, &rect, NULL, &position, white);
	spriteobj->End();
}

/* Animate texture (sprite sheet) */
void Sprite_Animate(int &frame, int startframe, int endframe, int direction, int &starttime, int delay)
{
	if ((int)GetTickCount() > starttime + delay)
	{
		starttime = GetTickCount();

		frame += direction;
		if (frame > endframe) frame = startframe;
		if (frame < startframe) frame = endframe;
	}
}

//// Collision detection
//int Collision(Sprite sprite1, Sprite sprite2)
//{
//	RECT rect1;
//	rect1.left = (long)sprite1.GetX();
//	rect1.top = (long)sprite1.GetY();
//	rect1.right = (long)sprite1.GetX() + sprite1.GetWidth();
//	rect1.bottom = (long)sprite1.GetY() + sprite1.GetHeight();
//
//	RECT rect2;
//	rect2.left = (long)sprite2.GetX();
//	rect2.top = (long)sprite2.GetY();
//	rect2.right = (long)sprite2.GetX() + sprite2.GetWidth();
//	rect2.bottom = (long)sprite2.GetY() + sprite2.GetHeight();
//
//	RECT dest;
//	return IntersectRect(&dest, &rect1, &rect2);
//}
//
//// Distance Collision
//bool CollisionD(Sprite sprite1, Sprite sprite2)
//{
//	double radius1, radius2;
//
//	// Calculate radius 1
//	if (sprite1.GetWidth() > sprite1.GetHeight())
//		radius1 = (sprite1.GetWidth) / 2.0;
//	else
//		radius1 = (sprite1.GetHeight()) / 2.0;
//
//	// Center point 1
//	double x1 = sprite1.GetX() + radius1;
//	double y1 = sprite1.GetY() + radius1;
//	D3DXVECTOR2 vector1(x1, y1);
//	// Calculate radius 2
//	if (sprite2.GetWidth() > sprite2.GetHeight())
//		radius2 = (sprite2.GetWidth()) / 2.0;
//	else
//		radius2 = (sprite2.GetHeight()) / 2.0;
//
//	// Center point 2
//	double x2 = sprite2.GetX() + radius2;
//	double y2 = sprite2.GetY() + radius2;
//	D3DXVECTOR2 vector2(x2, y2);
//
//	// Calculate distance
//	double deltax = vector1.x - vector2.x;
//	double deltay = vector2.y - vector1.y;
//	double dist = sqrt((deltax * deltax) + (deltay * deltay));
//
//	// Return distance comparison
//	return (dist < radius1 + radius2);
//}

/* DirectInput initialization */
bool DirectInput_Init(HWND hwnd)
{
	// Initialize DirectInput object
	HRESULT result = DirectInput8Create(
		GetModuleHandle(NULL),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&dinput,
		NULL);

	// Initialize the keyboard
	dinput->CreateDevice(GUID_SysKeyboard, &dikeyboard, NULL);
	dikeyboard->SetDataFormat(&c_dfDIKeyboard);
	dikeyboard->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	dikeyboard->Acquire();

	// Initialize the mouse
	dinput->CreateDevice(GUID_SysMouse, &dimouse, NULL);
	dimouse->SetDataFormat(&c_dfDIMouse);
	dimouse->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	dimouse->Acquire();
	d3ddev->ShowCursor(false);

	return true;
}

/* DirectInput update */
void DirectInput_Update()
{
	// Update mouse
	dimouse->GetDeviceState(sizeof(mouse_state), (LPVOID)&mouse_state);

	// Update keyboard
	dikeyboard->GetDeviceState(sizeof(keys), (LPVOID)&keys);

	// Update controllers
	for (int i = 0; i< 4; i++)
	{
		ZeroMemory(&controllers[i], sizeof(XINPUT_STATE));

		// Get the state of the controller
		XINPUT_STATE state;
		DWORD result = XInputGetState(i, &state);

		// Store state in global controllers array
		if (result == 0) controllers[i] = state.Gamepad;
	}

}

/* Return mouse x movement */
int Mouse_X()
{
	return mouse_state.lX;
}

/* Return mouse y movement */
int Mouse_Y()
{
	return mouse_state.lY;
}

/* Return mouse button state */
int Mouse_Button(int button)
{
	return mouse_state.rgbButtons[button] & 0x80;
}

/* Return key press state */
int Key_Down(int key)
{
	return (keys[key] & 0x80);
}

/* DirectInput shutdown */
void DirectInput_Shutdown()
{
	if (dikeyboard)
	{
		dikeyboard->Unacquire();
		dikeyboard->Release();
		dikeyboard = NULL;
	}
	if (dimouse)
	{
		dimouse->Unacquire();
		dimouse->Release();
		dimouse = NULL;
	}
}

/* Returns true if controller is plugged in */
bool XInput_Controller_Found()
{
	XINPUT_CAPABILITIES caps;
	ZeroMemory(&caps, sizeof(XINPUT_CAPABILITIES));
	XInputGetCapabilities(0, XINPUT_FLAG_GAMEPAD, &caps);
	if (caps.Type != 0) return false;

	return true;
}

/* Vibrates the controller */
void XInput_Vibrate(int contNum, int amount)
{
	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	vibration.wLeftMotorSpeed = amount;
	vibration.wRightMotorSpeed = amount;
	XInputSetState(contNum, &vibration);
}
