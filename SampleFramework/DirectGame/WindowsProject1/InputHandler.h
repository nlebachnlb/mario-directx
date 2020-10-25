#pragma once
#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <dinput.h>
#include "KeyEventHandler.h"
#include "Service.h"
#include "Consts.h"

class InputHandler : public Service
{
public:
	void Initialization();
	void SetHandleWindow(HWND hWnd);
	void SetKeyEventHandler(KeyEventHandler keyHandler);
	void ProcessKeyboard();

	bool GetKeyDown(int keyCode);
	bool GetKeyUp(int keyCode);
	
private:
	LPDIRECTINPUT8       di;								// The DirectInput object         
	LPDIRECTINPUTDEVICE8 didv;								// The keyboard device 

	BYTE  keyStates[256];									// DirectInput keyboard state buffer 
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];		// Buffered keyboard data

	KeyEventHandler keyEventHandler;
	HWND hWnd;
};
#endif