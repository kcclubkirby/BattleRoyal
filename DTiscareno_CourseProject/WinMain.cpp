//////////////////////////////////////////////////////////////////////////
// Name:	WinMain.h
// Date:	April 2nd, 2010
// Author:	Kyle Lauing [klauing@devry.edu] or [kylelauing@gmail.com]
// Purpose: This file is used to create a simple window application for
//			use with the DirectXFramework developed for the GSP 381 course
//			for DeVry University.
// Disclaimer:	
//			Copyright © 2010 by DeVry Educational Development Corporation.
//			All rights reserved.  No part of this work may be reproduced 
//			or used in any form or by any means – graphic, electronic, or 
//			mechanical, including photocopying, recording, Web distribution 
//			or information storage and retrieval systems – without the 
//			prior consent of DeVry Educational Development Corporation.
//////////////////////////////////////////////////////////////////////////
#include <iostream>
using namespace std;
#include <windows.h>
#define VC_EXTRALEAN

#include "DirectXFramework.h"

//////////////////////////////////////////////////////////////////////////
// Global Variables
//////////////////////////////////////////////////////////////////////////
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define WINDOW_TITLE L"Battle Royal - Course Project"

HWND				g_hWnd;			// Handle to the window
HINSTANCE			g_hInstance;	// Handle to the application instance
bool				g_bWindowed;	// Boolean for windowed or full-screen

//*************************************************************************
// This is where you declare the instance of your DirectXFramework Class
CDirectXFramework framework;

//*************************************************************************

// Entry point for the game or application.
int WINAPI wWinMain(HINSTANCE hInstance,	// Handle to the application
				   HINSTANCE hPrevInstance,	// Handle to the previous app
				   LPTSTR lpCmdLine,		// Command line string
				   int lpCmdShow);			// Show window value

LRESULT CALLBACK WndProc(HWND hWnd,			// Handle to the window
						 UINT message,		// Incoming Message
						 WPARAM wparam,		// Message Info
						 LPARAM lparam);	// Message Info

void InitWindow(void)
{
	WNDCLASSEX wndClass;  
	ZeroMemory(&wndClass, sizeof(wndClass));

	// set up the window
	wndClass.cbSize			= sizeof(WNDCLASSEX);			// size of window structure
	wndClass.lpfnWndProc	= (WNDPROC)WndProc;				// message callback
	wndClass.lpszClassName	= WINDOW_TITLE;					// class name
	wndClass.hInstance		= g_hInstance;					// handle to the application
	wndClass.hCursor		= LoadCursor(NULL, IDC_ARROW);	// default cursor
	wndClass.hbrBackground	= (HBRUSH)(COLOR_WINDOWFRAME);	// background brush

	// register a new type of window
	RegisterClassEx(&wndClass);

	g_hWnd = CreateWindow(
		WINDOW_TITLE, WINDOW_TITLE, 							// window class name and title
		g_bWindowed ? WS_OVERLAPPEDWINDOW | WS_VISIBLE:(WS_POPUP | WS_VISIBLE),// window style
		CW_USEDEFAULT, CW_USEDEFAULT,							// x and y coordinates
		SCREEN_WIDTH, SCREEN_HEIGHT,							// width and height of window
		NULL, NULL,												// parent window and menu
		g_hInstance,											// handle to application
		NULL);

	// Display the window
	ShowWindow(g_hWnd, SW_SHOW);
	UpdateWindow(g_hWnd);
}

int main()
{
	return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
	g_hInstance = hInstance;	// Store application handle
	g_bWindowed = true;			// Windowed mode or full-screen

	// Init the window
	InitWindow();

	// Use this msg structure to catch window messages
	MSG msg; 
	ZeroMemory(&msg, sizeof(msg));

	srand(timeGetTime());

	//*************************************************************************
	// Initialize DirectX/Game here (call the Init method of your framework)
	framework.Init(g_hWnd,g_hInstance,g_bWindowed);

	//*************************************************************************

	// Main Windows/Game Loop
	while(msg.message != WM_QUIT)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//*************************************************************************
		// This is where you call your DirectXFramework/Game render/update calls
		framework.Update();
		framework.Render();
		if (framework.Quit())
		{
			msg.message = WM_QUIT;
		}
		//*************************************************************************

	}

	//*************************************************************************
	//Shutdown DirectXFramework/Game here
	framework.Shutdown();

	//*************************************************************************

	// Unregister window
	UnregisterClass(WINDOW_TITLE, g_hInstance);

	// Return successful
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	// attempt to handle your messages
	switch(message)
	{
		case (WM_PAINT):
		{
			InvalidateRect(hWnd,NULL,TRUE);
			break;
		}		
		case(WM_DESTROY):
		{
			PostQuitMessage(0); 
			break;
		}
		case(WM_KEYDOWN):
		{
			switch(wparam)
			{
				// Escape Key will close the application
				// Remove/Modify this call when creating a real game with menus, etc.
				case VK_ESCAPE:
				{
					PostQuitMessage(0);
					break;
				}
			}
		}
	}

	// pass to default handler
	return DefWindowProc(hWnd, message, wparam, lparam);
}
