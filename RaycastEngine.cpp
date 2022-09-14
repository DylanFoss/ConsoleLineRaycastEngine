#include "RaycastEngine.h"
#include <chrono>
#include <vector>
#include <algorithm>
#include <iostream>
#include <math.h>

#include <debugapi.h>
#include <sstream>

#define PI 3.14159

// PLAYER/CAMERA SETTINGS

float playerX = 8.0f;
float playerY = 8.0f;
double playerA = 0.0f;
 
double fov = PI / 4.0f;
double rayDepth = 32;

bool correction = false;

// **********************

int mapWidth = 32;
int mapHeight = 32;

std::wstring map;

RaycastEngine::RaycastEngine()
{
	input = new InputHandler();
	console = nullptr;

	m_running = true;

	m_lastFrameTime = 0.0f;
	f_deltaTime = 0.0;
}

RaycastEngine::~RaycastEngine()
{
	delete console;
	delete input;
}

void RaycastEngine::constructConsole(int width, int height, int fontW, int fontH, std::wstring consoleTitle)
{
	console = new Console(width, height, fontW, fontH, consoleTitle);
}

void RaycastEngine::Run()
{
	if (console)
	{
		auto tp1 = std::chrono::system_clock().now();
		auto tp2 = std::chrono::system_clock().now();

		map += L"################################";
		map += L"#..............##..............#";
		map += L"#....######....##..............#";
		map += L"#..............##..............#";
		map += L"#....##..##....##..............#";
		map += L"#....#....#....##..............#";
		map += L"#..............................#";
		map += L"#..............................#";
		map += L"#....#....#....##..............#";
		map += L"#....##..##....##..............#";
		map += L"#..............##..............#";
		map += L"#..............##..............#";
		map += L"###.#############..............#";
		map += L"#.....#........##..............#";
		map += L"#.....#........##..............#";
		map += L"#.....#........#######....######";
		map += L"#.....#........#######....######";
		map += L"#.....#........##..............#";
		map += L"###.###........##....######....#";
		map += L"#..............##..............#";
		map += L"#....................##..##....#";
		map += L"#..............##....#....#....#";
		map += L"#..............................#";
		map += L"#..............##..............#";
		map += L"#....................#....#....#";
		map += L"#..............##....##..##....#";
		map += L"#..............................#";
		map += L"#..............##..............#";
		map += L"#..............##..#.#.#..#.#..#";
		map += L"#..............##..............#";
		map += L"#..............##..............#";
		map += L"################################";

		//console->Clear();

		while (m_running)
		{
			tp2 = std::chrono::system_clock::now();
			std::chrono::duration<float> elapsedTime = tp2 - tp1;
			tp1 = tp2;

			f_deltaTime = elapsedTime.count();

			input->handleKeyInputs();
			Update(f_deltaTime, input);
			Draw(console);
		}
	}

	delete this;
}

void RaycastEngine::Update(float f_deltaTime, InputHandler* input)
{

	//controls

	if (input->getKey('Q').bHeld)
	{
		playerA -= 1.0 * f_deltaTime;
		if (playerA < 0)
			playerA = PI * 2;
	}

	if (input->getKey('E').bHeld)
	{
		playerA += 1.0 * f_deltaTime;
		if (playerA >= PI * 2)
			playerA = 0;
	}

	if (input->getKey('W').bHeld)
	{
		playerX += sinf(playerA) * 5.0f * f_deltaTime;
		playerY += cosf(playerA) * 5.0f * f_deltaTime;

		if (map[(int)playerY * mapWidth + (int)playerX] == '#')
		{
			playerX -= sinf(playerA) * 5.0f * f_deltaTime;
			playerY -= cosf(playerA) * 5.0f * f_deltaTime;
		}
	}

	if (input->getKey('S').bHeld)
	{
		playerX -= sinf(playerA) * 5.0f * f_deltaTime;
		playerY -= cosf(playerA) * 5.0f * f_deltaTime;

		if (map[(int)playerY * mapWidth + (int)playerX] == '#')
		{
			playerX += sinf(playerA) * 5.0f * f_deltaTime;
			playerY += cosf(playerA) * 5.0f * f_deltaTime;
		}
	}

	if (input->getKey('A').bHeld)
	{
		playerX -= cosf(playerA) * 5.0f * f_deltaTime;
		playerY += sinf(playerA) * 5.0f * f_deltaTime;

		if (map[(int)playerY * mapWidth + (int)playerX] == '#')
		{
			playerX += cosf(playerA) * 5.0f * f_deltaTime;
			playerY -= sinf(playerA) * 5.0f * f_deltaTime;
		}
	}

	if (input->getKey('D').bHeld)
	{
		playerX += cosf(playerA) * 5.0f * f_deltaTime;
		playerY -= sinf(playerA) * 5.0f * f_deltaTime;

		if (map[(int)playerY * mapWidth + (int)playerX] == '#')
		{
			playerX -= cosf(playerA) * 5.0f * f_deltaTime;
			playerY += sinf(playerA) * 5.0f * f_deltaTime;
		}
	}

	if (input->getKey(VK_SPACE).bPressed)
		correction = !correction;

	for (int x = 0; x < ScreenWidth(); x++)
	{
		//raycast for each column on screen
		double rayAngle = (playerA - fov / 2.0f) + (((double)x / (double)ScreenWidth()) * fov);

		float rayDistance = 0;
		float rayHit = false;

		float boundary = false;

		float viewX = sinf(rayAngle); 
		float viewY = cosf(rayAngle);


		//DDA Algorithim

		double xStepSize = sqrt(1 + (viewY / viewX) * (viewY / viewX));
		double yStepSize = sqrt(1 + (viewX / viewY) * (viewX / viewY));

		int stepX;
		int stepY;

		int mapCheckX = playerX;
		int mapCheckY = playerY;

		float rayLengthX;
		float rayLengthY;

		float distance= 0;

		if (viewX < 0)
		{
			stepX = -1;
			rayLengthX = (playerX - float(mapCheckX)) * xStepSize;
		}
		else
		{
			stepX = 1;
			rayLengthX = (float(mapCheckX + 1) - playerX) * xStepSize;
		}
		if (viewY < 0)
		{
			stepY = -1;
			rayLengthY = (playerY - float(mapCheckY)) * yStepSize;
		}
		else
		{
			stepY = 1;
			rayLengthY = (float(mapCheckY + 1) - playerY) * yStepSize;
		}

		while (!rayHit && distance < rayDepth)
		{
			//walk
			if (rayLengthX < rayLengthY)
			{
				mapCheckX += stepX;
				distance = rayLengthX;
				rayLengthX += xStepSize;
			}
			else
			{
				mapCheckY += stepY;
				distance = rayLengthY;
				rayLengthY += yStepSize;
			}

			if (mapCheckX >= 0 || mapCheckX < mapWidth || mapCheckY >= 0 || mapCheckY < mapHeight)
			{
				if (map[mapCheckY * mapWidth + mapCheckX] == '#')
				{
					rayHit = true;
				}
			}

			float intersectionX;
			float intersectionY;

			if (rayHit)
			{
				intersectionX = playerX + viewX * distance;
				intersectionY = playerY + viewY * distance;

				rayDistance = sqrt((playerX - intersectionX) * (playerX - intersectionX) + (playerY - intersectionY) * (playerY - intersectionY));

				if(correction)
					rayDistance *= cos(rayAngle - playerA); //fisheye distortion fix (styistic choice)

				//int testX = (int)(playerX + viewX * rayDistance);
				//int testY = (int)(playerY + viewY * rayDistance);
			}
		}
		
		//ceiling and floor calculation
		int ceiling = (float)(ScreenHeight() / 2.0f) - ScreenHeight() / ((float)rayDistance);
		int floor = ScreenHeight() - ceiling;

		//shading based on distance
		short shade = ' ';
		short floorShade = ' ';

		if (rayDistance <= rayDepth / 4.0f)				shade = 0x2588;
		else if (rayDistance <= rayDepth / 3.0f)		shade = 0x2593;
		else if (rayDistance <= rayDepth / 2.0f)		shade = 0x2592;
		else if (rayDistance <= rayDepth)				shade = 0x2591;
		else											shade = ' ';

		if (boundary) shade = ' ';

		for (int y = 0; y < ScreenHeight(); y++)
		{
			if (y < ceiling)
			{
				console->Draw(x, y, L' ');
			}
			else if (y > ceiling && y <= floor)
			{
				console->Draw(x, y, shade, FG_DARK_BLUE);

				int borderValue = 3;


				if (rayDistance <= rayDepth / 4.0f)				 borderValue = 3;
				else if (rayDistance <= rayDepth / 3.0f)		 borderValue = 2;
				else if (rayDistance <= rayDepth / 2.0f)		 borderValue = 1;
				else if (rayDistance <= rayDepth)				 borderValue = 0;

				if (y < ceiling + borderValue || y > floor - borderValue)
				{
					console->Draw(x, y, shade, FG_GREY);
				}
			}
			else
			{
				float b = 1.0f - (((float)y - ScreenHeight() / 2.0f) / ((float)ScreenHeight() / 2.0f));
				if (b < 0.25)			floorShade = '#';
				else if (b < 0.5)		floorShade = 'x';
				else if (b < 0.75)		floorShade = '-';
				else if (b < 0.9)		floorShade = '.';
				else                    floorShade = ' ';
				console->Draw(x, y, floorShade);
			}
		}
	}

	//swprintf_s(screen, 40, L"x=%3.2f, Y=%3.2f, A=%3.2f, FPS=%3.2f ", playerX, playerY, playerA, (float)(1.0f / deltaTime));

	for (int nx = 0; nx < mapWidth; nx++)
	{
		for (int ny = 0; ny < mapHeight; ny++)
		{
			console->Draw(nx, ny+1, map[ny * mapWidth + (mapWidth - nx - 1)]);
		}
	}
	console->Draw((int)(mapWidth - playerX), ((int)playerY + 1), 'P');
}

void RaycastEngine::Draw(Console* console)
{
	console->Render(RaycastEngine::f_deltaTime);
}
