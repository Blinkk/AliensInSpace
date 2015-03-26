#include "Dx9Base.h"

Dx9Base::Dx9Base() {}

Dx9Base::Dx9Base(HWND window)
{
	_window = window;
}

Dx9Base::~Dx9Base()
{
	// Add deconstructor logic if needed...
	Shutdown();
}

bool Dx9Base::LoadContent()
{
	// This must be called in the overridden version of this function //
	//																
	//// Initialize Direct3D
	//if (!Direct3D_Init(_window, SCREENW, SCREENH, _fullscreen))	// !Fullscreen
	//	return false;
	//if (!DirectInput_Init(_window))
	//	return false;
	//
	//srand(time(NULL));		// Seed random generators

	// Override in derived class for specifics...
	return true;
}

void Dx9Base::UnloadContent()
{
	// Release memory here //
	// Override in derived class for specifics...
}

void Dx9Base::Update()
{
	// Add all update logic here...
	// Override in derived class for specifics...
}

void Dx9Base::Render()
{
	// Add all drawing logic here...
	// Override in derived class for specifics...
}

void Dx9Base::Shutdown()
{
	// Unload content
	UnloadContent();	// Calls overridden version (if one exists) 
						// for the class that calls this function

	// Release buffers, shaders, etc. if needed...

	Direct3D_Shutdown();
	DirectInput_Shutdown();
}