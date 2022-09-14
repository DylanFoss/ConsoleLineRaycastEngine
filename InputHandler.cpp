#include "InputHandler.h"
#include <String.h>
#include <Windows.h>

InputHandler::InputHandler()
{
	memset(keyNewState, 0, sizeof(short) * 256);
	memset(keyOldState, 0, sizeof(short) * 256);
	memset(keys, 0, sizeof(KeyState) * 256);
}

void InputHandler::handleKeyInputs()
{
	for (int i = 0; i < 256; i++)
	{
		keyNewState[i] = GetAsyncKeyState(i);

		keys[i].bPressed = false;
		keys[i].bReleased = false;

		if (keyNewState[i] != keyOldState[i])
		{
			if (keyNewState[i] & 0x8000)
			{
				keys[i].bPressed = !keys[i].bHeld;
				keys[i].bHeld = true;
			}
			else
			{
				keys[i].bReleased = true;
				keys[i].bHeld = false;
			}
		}

		keyOldState[i] = keyNewState[i];
	}
}
