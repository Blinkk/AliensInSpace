#ifndef DX9BASE_H
#define DX9BASE_H
#include "DirectX.h"

// Base class that can be used for any Dx9 Program
class Dx9Base
{
// Constant game values
protected:
	HWND _window;					// Game window (initialize in constructor)
	const bool _fullscreen = false;	// Flag for fullscreen / !fullscreen

public:
	Dx9Base();
	Dx9Base(HWND window);
	virtual ~Dx9Base();
	virtual bool LoadContent();
	virtual void Update();
	virtual void Render();
	virtual void UnloadContent();
	void Shutdown();				// Constant shutdown function 
									// (will call overridden UnloadContent())
};

#endif