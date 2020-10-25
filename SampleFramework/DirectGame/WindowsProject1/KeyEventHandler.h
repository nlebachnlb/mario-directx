#pragma once
#include <Windows.h>
#include <minwindef.h>

class CKeyEventHandler
{
public:
	virtual void KeyState(BYTE* state) = 0;
	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;
};

typedef CKeyEventHandler* KeyEventHandler;

class CGlobalKeyEventHandler : public CKeyEventHandler
{
public:
	virtual void KeyState(BYTE* state);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};