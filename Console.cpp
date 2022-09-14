#include "Console.h"

Console::Console(int width, int height, short fontH, short fontW, std::wstring name)
{
	console = GetStdHandle(STD_OUTPUT_HANDLE);
	consoleIn = GetStdHandle(STD_INPUT_HANDLE);

	screenWidth = width;
	screenHeight = height;

	consoleTitle = name;

	m_rectWindow = { 0, 0, 1, 1 };
	SetConsoleWindowInfo(console, TRUE, &m_rectWindow);

	// Set the size of the screen buffer
	COORD coord = { (short)screenWidth, (short)screenHeight };
	SetConsoleScreenBufferSize(console, coord);

	// Assign screen buffer to the console
	SetConsoleActiveScreenBuffer(console);

	CONSOLE_FONT_INFOEX fontInfo;
	fontInfo.cbSize = sizeof(fontInfo);
	fontInfo.nFont = 0;
	fontInfo.dwFontSize = { fontH, fontW };
	fontInfo.FontFamily = FF_DONTCARE;
	fontInfo.FontWeight = FW_NORMAL;
	wcscpy_s(fontInfo.FaceName, L"Consolas");

	SetCurrentConsoleFontEx(console, false, &fontInfo);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(console, &csbi);

	m_rectWindow = { 0, 0, (short)(screenWidth - 1),(short)(screenHeight - 1) };
	SetConsoleWindowInfo(console, TRUE, &m_rectWindow);

	SetConsoleMode(consoleIn, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

	screen = new CHAR_INFO[screenWidth * screenHeight];
	memset(screen, 0, sizeof(CHAR_INFO) * screenWidth * screenHeight);

}

Console::~Console()
{
	delete screen;
}

void Console::Render(float f_deltaTime)
{
	wchar_t s[256];
	swprintf_s(s, 256, L"%s - FPS: %3.2f", consoleTitle.c_str(), 1.0f / f_deltaTime);
	SetConsoleTitle(s);

	WriteConsoleOutput(console, screen, { (short)screenWidth, (short)screenHeight }, { 0,0 }, &m_rectWindow);
}
void Console::Clear()
{
	for (int i = 0; i < screenWidth * screenHeight; i++)
	{
		screen[i].Char.UnicodeChar = 0x0020;
		screen[i].Attributes = FG_WHITE;
	}
}

void Console::Draw(int x, int y, short c, short colour)
{
	if (x >= 0 && x < screenWidth && y >= 0 && y < screenHeight)
	{
		screen[y * screenWidth + x].Char.UnicodeChar = c;
		screen[y * screenWidth + x].Attributes = colour;
	}
}

void Console::DrawString(int x, int y, std::string string, short colour)
{
	if (x >= 0 && x < screenWidth && y >= 0 && y < screenHeight)
	{
		for (int i = 0; i < string.length(); i++)
		{
			screen[y * screenWidth + (x + i)].Char.UnicodeChar = string[i];
			screen[y * screenWidth + (x + i)].Attributes = colour;
		}
	}
}
;