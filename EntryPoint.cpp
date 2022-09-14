#pragma once
#include "RaycastEngine.h"

int main()
{
	RaycastEngine game;
	game.constructConsole(320, 240, 4, 4, L"Raycast Engine");
	// 
	//game.constructConsole(160, 120, 8, 8, L"Raycast Engine");
	//game.constructConsole(160, 120, 4, 4, L"Raycast Engine");

	//game.constructConsole(120, 40, 8, 16, L"Raycast Engine");
	game.Run();

	return 0;
}