#pragma once
#include "Console.h"
#include "InputHandler.h"

class RaycastEngine
{
public:

	RaycastEngine();
	~RaycastEngine();

	void constructConsole(int width, int height, int fontW = 8, int fontH = 18, std::wstring consoleTitle = L"Default");
	void Run();

	void Update(float f_deltaTime, InputHandler* input);
	void Draw(Console* console);

	bool running() const { return m_running; }
	bool quit() { m_running = false;  }

	Console* getConsole() const { return console; };

	int ScreenWidth() const { return console->getScreenWidth(); }
	int ScreenHeight() const { return console->getScreenHeight(); }

private:

	Console* debug;
	Console* console;
	InputHandler* input;

	bool m_running;
	float m_lastFrameTime;

protected:
	float f_deltaTime;
};

