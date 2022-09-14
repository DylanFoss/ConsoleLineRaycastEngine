#pragma once
class InputHandler
{
protected:

	//there are 254 keystates.
	struct KeyState
	{
		bool bPressed;
		bool bReleased;
		bool bHeld;
	} keys[256];

public:

	InputHandler();

	void handleKeyInputs();

	KeyState getKey(int keyID) { return keys[keyID]; };


private:

	short keyOldState[256] = { 0 };
	short keyNewState[256] = { 0 };
};

