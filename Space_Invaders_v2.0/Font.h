/**********************************
*  Programmer: Austin Ivicevic
*  Smoke Game Framework v1.2
***********************************/
/** Note: This class is closely modeled 
	after the book code from...
		Multi-Threaded Game Engine Design
		Copyright 2010 by Jonathan Harbour
**/

#pragma once
#include "DirectX.h"

class Font 
{
private:
    LPD3DXFONT fontObj;

public:
	Font();
    Font(std::string name, int size);
	~Font();
    void Print(int x, int y, std::string text, D3DCOLOR color);
    int getTextWidth(std::string text);
    int getTextHeight(std::string text);

};


