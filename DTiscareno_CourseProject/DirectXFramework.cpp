//////////////////////////////////////////////////////////////////////////
// Name:	DirectXFramework.cpp
// Date:	CREATED: April 2nd, 2010
//			MODIFIED: June 26th, 2013
// Author:	STARTED BY: Kyle Lauing [klauing@devry.edu] or [kylelauing@gml.com]
//          MODIFIED BY: David Tiscareno
// Purpose: This file is used to create a very simple framework for using
//			DirectX 9 for the GSP 381 course for DeVry University.
// Disclaimer:	
//			Copyright © 2010 by DeVry Educational Development Corporation.
//			All rights reserved.  No part of this work may be reproduced 
//			or used in any form or by any means – graphic, electronic, or 
//			mechanical, including photocopying, recording, Web distribution 
//			or information storage and retrieval systems – without the 
//			prior consent of DeVry Educational Development Corporation.
//////////////////////////////////////////////////////////////////////////

#include "DirectXFramework.h"

CDirectXFramework::CDirectXFramework(void)
{
	// Init or NULL objects before use to avoid any undefined behavior
	m_bVsync		= false;
	m_pD3DObject	= 0;
	m_pD3DDevice	= 0;
}

CDirectXFramework::~CDirectXFramework(void)
{
	// If Shutdown is not explicitly called correctly, call it when 
	// this class is destroyed or falls out of scope as an error check.
	Shutdown();
}

void CDirectXFramework::Init(HWND& hWnd, HINSTANCE& hInst, bool bWindowed)
{
	m_hWnd = hWnd;
	m_hInst = hInst;
	windowed = bWindowed;
	quit = false;

	//////////////////////////////////////////////////////////////////////////
	// Direct3D Foundations - D3D Object, Present Parameters, and D3D Device
	//////////////////////////////////////////////////////////////////////////

	// Create the D3D Object
	m_pD3DObject = Direct3DCreate9(D3D_SDK_VERSION);

	// Find the width and height of window using hWnd and GetWindowRect()
	RECT rect;
	GetWindowRect(hWnd, &rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	// Set D3D Device presentation parameters before creating the device
	
	ZeroMemory(&D3Dpp, sizeof(D3Dpp));  // NULL the structure's memory

	D3Dpp.hDeviceWindow					= hWnd;										// Handle to the focus window
	D3Dpp.Windowed						= bWindowed;								// Windowed or Full-screen boolean
	D3Dpp.AutoDepthStencilFormat		= D3DFMT_D24S8;								// Format of depth/stencil buffer, 24 bit depth, 8 bit stencil
	D3Dpp.EnableAutoDepthStencil		= TRUE;										// Enables Z-Buffer (Depth Buffer)
	D3Dpp.BackBufferCount				= 1;										// Change if need of > 1 is required at a later date
	D3Dpp.BackBufferFormat				= D3DFMT_X8R8G8B8;							// Back-buffer format, 8 bits for each pixel
	D3Dpp.BackBufferHeight				= height;									// Make sure resolution is supported, use adapter modes
	D3Dpp.BackBufferWidth				= width;									// (Same as above)
	D3Dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;					// Discard back-buffer, must stay discard to support multi-sample
	D3Dpp.PresentationInterval			= m_bVsync ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE; // Present back-buffer immediately, unless V-Sync is on								
	D3Dpp.Flags							= D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;		// This flag should improve performance, if not set to NULL.
	D3Dpp.FullScreen_RefreshRateInHz	= bWindowed ? 0 : D3DPRESENT_RATE_DEFAULT;	// Full-screen refresh rate, use adapter modes or default
	D3Dpp.MultiSampleQuality			= 0;										// MSAA currently off, check documentation for support.
	D3Dpp.MultiSampleType				= D3DMULTISAMPLE_NONE;						// MSAA currently off, check documentation for support.

	// Check device capabilities
	DWORD deviceBehaviorFlags = 0;
	m_pD3DObject->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_D3DCaps);

	// Determine vertex processing mode
	if(m_D3DCaps.DevCaps & D3DCREATE_HARDWARE_VERTEXPROCESSING)
	{
		// Hardware vertex processing supported? (Video Card)
		deviceBehaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;	
	}
	else
	{
		// If not, use software (CPU)
		deviceBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING; 
	}
	
	// If hardware vertex processing is on, check pure device support
	if(m_D3DCaps.DevCaps & D3DDEVCAPS_PUREDEVICE && deviceBehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING)
	{
		deviceBehaviorFlags |= D3DCREATE_PUREDEVICE;	
	}
	
	// Create the D3D Device with the present parameters and device flags above
	m_pD3DObject->CreateDevice(
		D3DADAPTER_DEFAULT,		// which adapter to use, set to primary
		D3DDEVTYPE_HAL,			// device type to use, set to hardware rasterization
		hWnd,					// handle to the focus window
		deviceBehaviorFlags,	// behavior flags
		&D3Dpp,					// presentation parameters
		&m_pD3DDevice);			// returned device pointer

	//*************************************************************************

	//////////////////////////////////////////////////////////////////////////
	// Create a Font Object
	//////////////////////////////////////////////////////////////////////////
	
	// Load a font for private use for this process


	// Load D3DXFont, each font style you want to support will need an ID3DXFont

	D3DXCreateFontW(m_pD3DDevice,22,0,FW_NORMAL,1,false,DEFAULT_CHARSET,
				   OUT_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,
				   L"Arial",&m_pD3DFont);

	//////////////////////////////////////////////////////////////////////////
	// Create Sprite Object and Textures
	//////////////////////////////////////////////////////////////////////////

	// Create a sprite object, note you will only need one for all 2D sprites
	D3DXCreateSprite(m_pD3DDevice, &m_pD3DSprite);

	// Create a texture, each different 2D sprite to display to the screen
	// will need a new texture object.  If drawing the same sprite texture
	// multiple times, just call that sprite's Draw() with different 
	// transformation values.

	D3DXCreateTextureFromFileEx(m_pD3DDevice,L"MainMenu.png",800,600,0,0,D3DFMT_UNKNOWN,
								D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
								0,&menuBackInfo,0,
								&menuBackground);
	menuBackgroundSetup.Initialize();
	D3DXCreateTextureFromFileEx(m_pD3DDevice,L"BattleRoyal.png",300,100,0,0,D3DFMT_UNKNOWN,
								D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
								0,&titleTex_I,0,
								&titleTex);
	titleTex_Set.Initialize();
	titleTex_Set.transMat._41 = 150;
	titleTex_Set.transMat._42 = 150;
	D3DXCreateTextureFromFileEx(m_pD3DDevice,L"PlayGameButton.png",134,41,0,0,D3DFMT_UNKNOWN,
								D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
								D3DCOLOR_XRGB(255,0,255),&playButtonTex_I,0,
								&playButtonTex);
	playButtonTex_Set.Initialize();
	playButtonTex_Set.transMat._41 = 700;
	playButtonTex_Set.transMat._42 = 200;
	D3DXCreateTextureFromFileEx(m_pD3DDevice,L"PlayGameButton_S.png",134,41,0,0,D3DFMT_UNKNOWN,
								D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
								D3DCOLOR_XRGB(255,0,255),&playSelectTex_I,0,
								&playSelectTex);
	playSelectTex_Set.Initialize();
	playSelectTex_Set.transMat._41 = 700;
	playSelectTex_Set.transMat._42 = 200;
	D3DXCreateTextureFromFileEx(m_pD3DDevice,L"CreditsButton.png",134,41,0,0,D3DFMT_UNKNOWN,
								D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
								D3DCOLOR_XRGB(255,0,255),&credButtonTex_I,0,
								&credButtonTex);
	credButtonTex_Set.Initialize();
	credButtonTex_Set.transMat._41 = 700;
	credButtonTex_Set.transMat._42 = 300;
	D3DXCreateTextureFromFileEx(m_pD3DDevice,L"CreditsButton_S.png",134,41,0,0,D3DFMT_UNKNOWN,
								D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
								D3DCOLOR_XRGB(255,0,255),&credSelectTex_I,0,
								&credSelectTex);
	credSelectTex_Set.Initialize();
	credSelectTex_Set.transMat._41 = 700;
	credSelectTex_Set.transMat._42 = 300;
	D3DXCreateTextureFromFileEx(m_pD3DDevice,L"OptionsButton.png",134,41,0,0,D3DFMT_UNKNOWN,
								D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
								D3DCOLOR_XRGB(255,0,255),&optButtonTex_I,0,
								&optButtonTex);
	optButtonTex_Set.Initialize();
	optButtonTex_Set.transMat._41 = 700;
	optButtonTex_Set.transMat._42 = 400;
	D3DXCreateTextureFromFileEx(m_pD3DDevice,L"OptionsButton_S.png",134,41,0,0,D3DFMT_UNKNOWN,
								D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
								D3DCOLOR_XRGB(255,0,255),&optSelectTex_I,0,
								&optSelectTex);
	optSelectTex_Set.Initialize();
	optSelectTex_Set.transMat._41 = 700;
	optSelectTex_Set.transMat._42 = 400;
	D3DXCreateTextureFromFileEx(m_pD3DDevice,L"QuitButton.png",134,41,0,0,D3DFMT_UNKNOWN,
								D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
								D3DCOLOR_XRGB(255,0,255),&quitButtonTex_I,0,
								&quitButtonTex);
	quitButtonTex_Set.Initialize();
	quitButtonTex_Set.transMat._41 = 700;
	quitButtonTex_Set.transMat._42 = 500;
	D3DXCreateTextureFromFileEx(m_pD3DDevice,L"QuitButton_S.png",134,41,0,0,D3DFMT_UNKNOWN,
								D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
								D3DCOLOR_XRGB(255,0,255),&quitSelectTex_I,0,
								&quitSelectTex);
	quitSelectTex_Set.Initialize();
	quitSelectTex_Set.transMat._41 = 700;
	quitSelectTex_Set.transMat._42 = 500;
	D3DXCreateTextureFromFileEx(m_pD3DDevice,L"BackButton.png",134,41,0,0,D3DFMT_UNKNOWN,
								D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
								D3DCOLOR_XRGB(255,0,255),&backButtonTex_I,0,
								&backButtonTex);
	backButtonTex_Set.Initialize();
	backButtonTex_Set.transMat._41 = 150;
	backButtonTex_Set.transMat._42 = 100;
	D3DXCreateTextureFromFileEx(m_pD3DDevice,L"BackButton_S.png",134,41,0,0,D3DFMT_UNKNOWN,
								D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
								D3DCOLOR_XRGB(255,0,255),&backSelectTex_I,0,
								&backSelectTex);
	backSelectTex_Set.Initialize();
	backSelectTex_Set.transMat._41 = 150;
	backSelectTex_Set.transMat._42 = 100;
	D3DXCreateTextureFromFileEx(m_pD3DDevice,L"FullScreenButton.png",134,41,0,0,D3DFMT_UNKNOWN,
								D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
								D3DCOLOR_XRGB(255,0,255),&fScreenButtonTex_I,0,
								&fScreenButtonTex);
	fScreenButtonTex_Set.Initialize();
	fScreenButtonTex_Set.transMat._41 = 300;
	fScreenButtonTex_Set.transMat._42 = 300;
	D3DXCreateTextureFromFileEx(m_pD3DDevice,L"FullScreenButton_S.png",134,41,0,0,D3DFMT_UNKNOWN,
								D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
								D3DCOLOR_XRGB(255,0,255),&fScreenSelectTex_I,0,
								&fScreenSelectTex);
	fScreenSelectTex_Set.Initialize();
	fScreenSelectTex_Set.transMat._41 = 300;
	fScreenSelectTex_Set.transMat._42 = 300;
	D3DXCreateTextureFromFileEx(m_pD3DDevice,L"WindowedButton.png",134,41,0,0,D3DFMT_UNKNOWN,
								D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
								D3DCOLOR_XRGB(255,0,255),&windButtonTex_I,0,
								&windButtonTex);
	windButtonTex_Set.Initialize();
	windButtonTex_Set.transMat._41 = 450;
	windButtonTex_Set.transMat._42 = 300;
	D3DXCreateTextureFromFileEx(m_pD3DDevice,L"WindowedButton_S.png",134,41,0,0,D3DFMT_UNKNOWN,
								D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
								D3DCOLOR_XRGB(255,0,255),&windSelectTex_I,0,
								&windSelectTex);
	windSelectTex_Set.Initialize();
	windSelectTex_Set.transMat._41 = 450;
	windSelectTex_Set.transMat._42 = 300;
	D3DXCreateTextureFromFileEx(m_pD3DDevice,L"MainMenuButton.png",134,41,0,0,D3DFMT_UNKNOWN,
								D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
								D3DCOLOR_XRGB(255,0,255),&mainButtonTex_I,0,
								&mainButtonTex);
	mainButtonTex_Set.Initialize();
	mainButtonTex_Set.transMat._41 = 700;
	mainButtonTex_Set.transMat._42 = 250;
	D3DXCreateTextureFromFileEx(m_pD3DDevice,L"MainMenuButton_S.png",134,41,0,0,D3DFMT_UNKNOWN,
								D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
								D3DCOLOR_XRGB(255,0,255),&mainSelectTex_I,0,
								&mainSelectTex);
	mainSelectTex_Set.Initialize();
	mainSelectTex_Set.transMat._41 = 700;
	mainSelectTex_Set.transMat._42 = 250;
	D3DXCreateTextureFromFileEx(m_pD3DDevice,L"RestartButton.png",134,41,0,0,D3DFMT_UNKNOWN,
								D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
								D3DCOLOR_XRGB(255,0,255),&restartButtonTex_I,0,
								&restartButtonTex);
	restartButtonTex_Set.Initialize();
	restartButtonTex_Set.transMat._41 = 700;
	restartButtonTex_Set.transMat._42 = 350;
	D3DXCreateTextureFromFileEx(m_pD3DDevice,L"RestartButton_S.png",134,41,0,0,D3DFMT_UNKNOWN,
								D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
								D3DCOLOR_XRGB(255,0,255),&restartSelectTex_I,0,
								&restartSelectTex);
	restartSelectTex_Set.Initialize();
	restartSelectTex_Set.transMat._41 = 700;
	restartSelectTex_Set.transMat._42 = 350;
	D3DXCreateTextureFromFileEx(m_pD3DDevice,L"Credits.png",478,192,0,0,D3DFMT_UNKNOWN,
								D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
								D3DCOLOR_XRGB(255,0,255),&creditsTex_I,0,
								&creditsTex);
	creditsTex_Set.Initialize();
	creditsTex_Set.transMat._41 = 450;
	creditsTex_Set.transMat._42 = 500;
	D3DXCreateTextureFromFileEx(m_pD3DDevice,L"Level1.png",250,75,0,0,D3DFMT_UNKNOWN,
								D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
								D3DCOLOR_XRGB(255,0,255),&LevelOneTex_I,0,
								&LevelOneTex);
	LevelOneTex_Set.Initialize();
	LevelOneTex_Set.transMat._41 = 675;
	LevelOneTex_Set.transMat._42 = 150;
	D3DXCreateTextureFromFileEx(m_pD3DDevice,L"Level2.png",250,75,0,0,D3DFMT_UNKNOWN,
								D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
								D3DCOLOR_XRGB(255,0,255),&LevelTwoTex_I,0,
								&LevelTwoTex);
	LevelTwoTex_Set.Initialize();
	LevelTwoTex_Set.transMat._41 = 675;
	LevelTwoTex_Set.transMat._42 = 150;
	D3DXCreateTextureFromFileEx(m_pD3DDevice,L"Level3.png",250,75,0,0,D3DFMT_UNKNOWN,
								D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
								D3DCOLOR_XRGB(255,0,255),&LevelThreeTex_I,0,
								&LevelThreeTex);
	LevelThreeTex_Set.Initialize();
	LevelThreeTex_Set.transMat._41 = 675;
	LevelThreeTex_Set.transMat._42 = 150;
	D3DXCreateTextureFromFileEx(m_pD3DDevice,L"Level4.png",250,75,0,0,D3DFMT_UNKNOWN,
								D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
								D3DCOLOR_XRGB(255,0,255),&LevelFourTex_I,0,
								&LevelFourTex);
	LevelFourTex_Set.Initialize();
	LevelFourTex_Set.transMat._41 = 675;
	LevelFourTex_Set.transMat._42 = 150;
	D3DXCreateTextureFromFileEx(m_pD3DDevice,L"Level5.png",250,75,0,0,D3DFMT_UNKNOWN,
								D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
								D3DCOLOR_XRGB(255,0,255),&LevelFiveTex_I,0,
								&LevelFiveTex);
	LevelFiveTex_Set.Initialize();
	LevelFiveTex_Set.transMat._41 = 675;
	LevelFiveTex_Set.transMat._42 = 150;
	D3DXCreateTextureFromFileEx(m_pD3DDevice,L"YouWin.png",350,105,0,0,D3DFMT_UNKNOWN,
								D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
								D3DCOLOR_XRGB(255,0,255),&WinTex_I,0,
								&WinTex);
	WinTex_Set.Initialize();
	WinTex_Set.transMat._41 = 350;
	WinTex_Set.transMat._42 = 400;
	D3DXCreateTextureFromFileEx(m_pD3DDevice,L"BasicTile.png",50,50,0,0,D3DFMT_UNKNOWN,
								D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
								D3DCOLOR_XRGB(255,0,255),&basicTileTex_I,0,
								&basicTileTex);
	basicTileTex_Set.Initialize();
	//*************************************************************************

	gameState = LOAD;

	//Create the filter graph
	CoInitialize(NULL);
	CoCreateInstance( CLSID_FilterGraph,NULL,CLSCTX_INPROC_SERVER,IID_IGraphBuilder,(void**)&m_pGraphBuilder);

	//Create the control and event objects
	m_pGraphBuilder->QueryInterface(IID_IMediaControl,
                                (void**)&m_pMediaControl);
	m_pGraphBuilder->QueryInterface(IID_IMediaEvent,
                                (void**)&m_pMediaEvent);

	//Build the filter graph for the video
	m_pGraphBuilder->RenderFile(L"Intro.avi", NULL);

    // Video Window
    m_pMediaControl->QueryInterface(IID_IVideoWindow,
                                    (void**)&m_pVideoWindow);
    // Setup the window
    m_pVideoWindow->put_Owner((OAHWND)m_hWnd);
    // Set the style
    m_pVideoWindow->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE);
    // Obtain the size of the window
    RECT WinRect;
    GetClientRect(m_hWnd, &WinRect);
    // Set the video size to the size of the window
    m_pVideoWindow->SetWindowPosition(WinRect.left, WinRect.top,
                                        WinRect.right, WinRect.bottom);

	//set up the Direct Input
	DirectInput8Create(hInst,DIRECTINPUT_VERSION,IID_IDirectInput8, (void**) &m_pDIObject, NULL);

	//Create the direct input devices
	m_pDIObject->CreateDevice(GUID_SysKeyboard, &m_pDIKeyboard, NULL);
	m_pDIObject->CreateDevice(GUID_SysMouse, &m_pDIMouse, NULL);

	//Set their data formats
	m_pDIKeyboard->SetDataFormat(&c_dfDIKeyboard);
	m_pDIMouse->SetDataFormat(&c_dfDIMouse2);

	//Set their cooperative level
	m_pDIKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	m_pDIMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE); 

	int result;
	keyboard_delay = 0;
	maxGrid = 9;
	moving = false;
	deadCounter = -1;
	pulseCount = 10;

	result = FMOD::System_Create(&system);
	ERRCHECK(result);
	system->init(100,FMOD_INIT_NORMAL,0);
	result = system->createStream("PhoreiaTitle.mp3",
									FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &myStream);
	ERRCHECK(result); // displays message if createStream returned an error
	result = system->createStream("PhoreiaEnd.mp3",
									FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &gameStream);
	ERRCHECK(result); // displays message if createStream returned an error
	result = system->createSound("Attack1.ogg",
									FMOD_2D | FMOD_HARDWARE, 0, &sound1);
	ERRCHECK(result); // displays message if createStream returned an error
	result = system->createSound("Attack2.ogg",
									FMOD_2D | FMOD_HARDWARE, 0, &sound2);
	ERRCHECK(result); // displays message if createStream returned an error
	result = system->createSound("Cursor.ogg",
									FMOD_2D | FMOD_HARDWARE, 0, &sound3);
	ERRCHECK(result); // displays message if createStream returned an error
	result = system->createSound("Lose.ogg",
									FMOD_2D | FMOD_HARDWARE, 0, &sound4);
	ERRCHECK(result); // displays message if createStream returned an error
	result = system->createSound("Select.ogg",
									FMOD_2D | FMOD_HARDWARE, 0, &sound5);
	ERRCHECK(result); // displays message if createStream returned an error
	result = system->createSound("Win.ogg",
									FMOD_2D | FMOD_HARDWARE, 0, &sound6);
	ERRCHECK(result); // displays message if createStream returned an error
}

void CDirectXFramework::Update()
{
	//Acquire the direct input
	m_pDIKeyboard->Acquire();
	m_pDIMouse->Acquire();

	//Save the direct input states
	HRESULT hr = m_pDIKeyboard->GetDeviceState(sizeof(buffer), (LPVOID) &buffer);
	if (FAILED(hr))
	{
		// Keyboard lost, zero out keyboard data structure.
		ZeroMemory(buffer, sizeof(buffer));

		// Try to acquire for next time we poll.
		hr = m_pDIKeyboard->Acquire();
	}
	hr = m_pDIMouse->GetDeviceState(sizeof(mouseState), &mouseState);
	if (FAILED(hr))
	{
		// Mouse lost, zero out mouse data structure.
		ZeroMemory(&mouseState, sizeof(mouseState));

		// Try to acquire for next time we poll.
		hr = m_pDIMouse->Acquire();
	}
	if (keyboard_delay != 0)
		keyboard_delay -= 0.01;
	if (keyboard_delay <= 0)
		keyboard_delay = 0;

	//add a frame to the counter
	lastTickCount = currentTickCount; //reset the time counter
	currentTickCount = clock() * 0.001f;

	if (gameState == GAME)
	{
		if (deadCounter != -1)
		{
			deadCounter -= 0.01;
			if (deadCounter <= 0)
			{
				deadCounter = -1;
				if (levelNum == 1)
					levelState = LOAD1;
				if (levelNum == 2)
					levelState = LOAD2;
				if (levelNum == 3)
					levelState = LOAD3;
				if (levelNum == 4)
					levelState = LOAD4;
				if (levelNum == 5)
					levelState = LOAD5;
				int result = myChannel->setVolume(1);
			}
		}
		if (!moving)
		{
			ai_1.Pulse(m_pD3DDevice,pulseCount);
			ai_2.Pulse(m_pD3DDevice,pulseCount);
			ai_3.Pulse(m_pD3DDevice,pulseCount);
			ai_4.Pulse(m_pD3DDevice,pulseCount);
			ai_5.Pulse(m_pD3DDevice,pulseCount);
			ai_6.Pulse(m_pD3DDevice,pulseCount);
			ai_7.Pulse(m_pD3DDevice,pulseCount);
			ai_8.Pulse(m_pD3DDevice,pulseCount);
			ai_9.Pulse(m_pD3DDevice,pulseCount);
			ai_10.Pulse(m_pD3DDevice,pulseCount);
			pulseCount -= 0.01;
			if (pulseCount <= 0)
			{
				pulseCount = 10;
			}

			if(playerDying)
			{
				player = Player();
				int result = myChannel->setVolume(0.2);
				ERRCHECK(result); // displays message if this returned an error
				result = system->playSound(FMOD_CHANNEL_FREE, sound2, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				result = system->playSound(FMOD_CHANNEL_FREE, sound4, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				deadCounter = 5000;
				playerDying = false;
			}
			player.checkIfMove(maxGrid, playerPos,grid,collision,m_pD3DDevice);
			if (collision == -1)
				moving = player.Move(grid,playerPos);
			else
			{
				moving = CheckPCollision();
			}
			if (moving)
			{
				turnCounter = 0.5;
				pulseCount = 10;
				ai_1.Pulse(m_pD3DDevice,pulseCount);
				ai_2.Pulse(m_pD3DDevice,pulseCount);
				ai_3.Pulse(m_pD3DDevice,pulseCount);
				ai_4.Pulse(m_pD3DDevice,pulseCount);
				ai_5.Pulse(m_pD3DDevice,pulseCount);
				ai_6.Pulse(m_pD3DDevice,pulseCount);
				ai_7.Pulse(m_pD3DDevice,pulseCount);
				ai_8.Pulse(m_pD3DDevice,pulseCount);
				ai_9.Pulse(m_pD3DDevice,pulseCount);
				ai_10.Pulse(m_pD3DDevice,pulseCount);
				playerDying |= ai_1.Move(grid,aI1Pos,maxGrid,playerPos,m_pD3DDevice);
				playerDying |= ai_2.Move(grid,aI2Pos,maxGrid,playerPos,m_pD3DDevice);
				playerDying |= ai_3.Move(grid,aI3Pos,maxGrid,playerPos,m_pD3DDevice);
				playerDying |= ai_4.Move(grid,aI4Pos,maxGrid,playerPos,m_pD3DDevice);
				playerDying |= ai_5.Move(grid,aI5Pos,maxGrid,playerPos,m_pD3DDevice);
				playerDying |= ai_6.Move(grid,aI6Pos,maxGrid,playerPos,m_pD3DDevice);
				playerDying |= ai_7.Move(grid,aI7Pos,maxGrid,playerPos,m_pD3DDevice);
				playerDying |= ai_8.Move(grid,aI8Pos,maxGrid,playerPos,m_pD3DDevice);
				playerDying |= ai_9.Move(grid,aI9Pos,maxGrid,playerPos,m_pD3DDevice);
				playerDying |= ai_10.Move(grid,aI10Pos,maxGrid,playerPos,m_pD3DDevice);
			}
		}
		else
		{
			bool stillMoving = false;
			stillMoving |= player.Update(currentTickCount - lastTickCount,playerPos);
			stillMoving |= ai_1.Update(currentTickCount - lastTickCount,aI1Pos);
			stillMoving |= ai_2.Update(currentTickCount - lastTickCount,aI2Pos);
			stillMoving |= ai_3.Update(currentTickCount - lastTickCount,aI3Pos);
			stillMoving |= ai_4.Update(currentTickCount - lastTickCount,aI4Pos);
			stillMoving |= ai_5.Update(currentTickCount - lastTickCount,aI5Pos);
			stillMoving |= ai_6.Update(currentTickCount - lastTickCount,aI6Pos);
			stillMoving |= ai_7.Update(currentTickCount - lastTickCount,aI7Pos);
			stillMoving |= ai_8.Update(currentTickCount - lastTickCount,aI8Pos);
			stillMoving |= ai_9.Update(currentTickCount - lastTickCount,aI9Pos);
			stillMoving |= ai_10.Update(currentTickCount - lastTickCount,aI10Pos);
			turnCounter -= currentTickCount - lastTickCount;
			if (turnCounter > 0)
				stillMoving = true;
			moving = stillMoving;
		}
		if (AllEnemiesDead() && !levelChanging)
		{
			levelChanging = true;
			int result = myChannel->setVolume(0.2);
			ERRCHECK(result); // displays message if this returned an error
			result = system->playSound(FMOD_CHANNEL_FREE, sound6, false, &myChannel2);
			ERRCHECK(result);
			result = myChannel2->setVolume(0.5);
			ERRCHECK(result); // displays message if this returned an error
			turnCounter = 3;
		}
		if (levelChanging)
		{
			turnCounter -= currentTickCount - lastTickCount;
			if (turnCounter <= 0)
			{
				levelNum++;
				if (levelNum == 2)
					levelState = LOAD2;
				if (levelNum == 3)
					levelState = LOAD3;
				if (levelNum == 4)
					levelState = LOAD4;
				if (levelNum == 5)
					levelState = LOAD5;
				if (levelNum >= 6)
					gameWin = true;
				int result = myChannel->setVolume(1);
				ERRCHECK(result); // displays message if this returned an error
			}
		}
	}
	system->update();
}

void CDirectXFramework::Render()
{
	// If the device was not created successfully, return
	if(!m_pD3DDevice)
		return;
	//*************************************************************************

	if(gameState == LOAD)
	{
		m_pMediaControl->Run();
		gameState = INTRO;
	}

	if (gameState == INTRO)
	{
		long evCode;
		LONG_PTR eventParam1, eventParam2;
		m_pMediaEvent->GetEvent(&evCode, &eventParam1, &eventParam2,1);
		if (buffer[DIK_RETURN] & 0x80)
		{
			m_pMediaControl->Stop();
			m_pVideoWindow->put_Visible(OAFALSE);
			m_pVideoWindow->put_Owner((OAHWND)m_hWnd);
			m_pMediaControl->Release();
			m_pMediaEvent->Release();
			m_pGraphBuilder->Release(); // Should be AFTER the other calls!
			CoUninitialize();
			int result;
			result = system->playSound(FMOD_CHANNEL_FREE, myStream,
                           false, &myChannel);
			ERRCHECK(result);
			buttonSelect = MAIN;
			gameState = MENU;
			menuState = MAIN;
		}
		if (evCode == EC_COMPLETE)
		{
			m_pVideoWindow->put_Visible(OAFALSE);
			m_pVideoWindow->put_Owner((OAHWND)m_hWnd);
			m_pMediaControl->Release();
			m_pMediaEvent->Release();
			m_pGraphBuilder->Release(); // Should be AFTER the other calls!
			CoUninitialize();
			int result;
			result = system->playSound(FMOD_CHANNEL_FREE, myStream,
                           false, &myChannel);
			ERRCHECK(result);
			buttonSelect = MAIN;
			gameState = MENU;
			menuState = MAIN;
		}
	}
	if (gameState == MENU)
	{
		//////////////////////////////////////////////////////////////////////////
		// All draw calls between swap chain's functions, and pre-render and post- 
		// render functions (Clear and Present, BeginScene and EndScene)
		//////////////////////////////////////////////////////////////////////////

		// Clear the back buffer, call BeginScene()
		m_pD3DDevice->Clear(0,0,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(255,255,255), 1.0f, 0);
		m_pD3DDevice->BeginScene();

		m_pD3DSprite->Begin(D3DXSPRITE_ALPHABLEND);

		D3DXVECTOR3 center(0,0,0); //set the center of the sprite
		m_pD3DSprite->SetTransform(&(menuBackgroundSetup.CreateWorldMat()));
		m_pD3DSprite->Draw(menuBackground,0,&center,0,
						   D3DCOLOR_XRGB(255,255,255)); //draw the sprite

		POINT mousePos;
		bool mouseOff = true;
		
		GetCursorPos(&mousePos);
		ScreenToClient(m_hWnd,&mousePos);

		if (menuState == MAIN)
		{
			center = D3DXVECTOR3(titleTex_I.Width/2.0f,titleTex_I.Width/2.0f,0); //set the center of the sprite
			m_pD3DSprite->SetTransform(&(titleTex_Set.CreateWorldMat()));
			m_pD3DSprite->Draw(titleTex,0,&center,0,
							   D3DCOLOR_XRGB(255,255,255)); //draw the sprite

			if (mousePos.x > (playButtonTex_Set.transMat._41 - playSelectTex_I.Width/2.0f)-20 &&
				mousePos.x < (playButtonTex_Set.transMat._41 + playSelectTex_I.Width/2.0f)-15 &&
				mousePos.y > (playButtonTex_Set.transMat._42 - playSelectTex_I.Height/2.0f)-60&&
				mousePos.y < (playButtonTex_Set.transMat._42 + playSelectTex_I.Height/2.0f)-60)
			{
				if (buttonSelect != PLAY)
				{
					int result = system->playSound(FMOD_CHANNEL_FREE, sound3, false, &myChannel2);
					ERRCHECK(result);
					result = myChannel2->setVolume(0.5);
					ERRCHECK(result); // displays message if this returned an error
				}
				buttonSelect = PLAY;
				mouseOff = false;
			}
			else if (	mousePos.x > (credButtonTex_Set.transMat._41 - credSelectTex_I.Width/2.0f)-20 &&
						mousePos.x < (credButtonTex_Set.transMat._41 + credSelectTex_I.Width/2.0f)-15 &&
						mousePos.y > (credButtonTex_Set.transMat._42 - credSelectTex_I.Height/2.0f)-64&&
						mousePos.y < (credButtonTex_Set.transMat._42 + credSelectTex_I.Height/2.0f)-64)
			{
				if (buttonSelect != CREDITS)
				{
					int result = system->playSound(FMOD_CHANNEL_FREE, sound3, false, &myChannel2);
					ERRCHECK(result);
					result = myChannel2->setVolume(0.5);
					ERRCHECK(result); // displays message if this returned an error
				}
				buttonSelect = CREDITS;
				mouseOff = false;
			}
			else if (	mousePos.x > (optButtonTex_Set.transMat._41 - optSelectTex_I.Width/2.0f)-20 &&
						mousePos.x < (optButtonTex_Set.transMat._41 + optSelectTex_I.Width/2.0f)-15 &&
						mousePos.y > (optButtonTex_Set.transMat._42 - optSelectTex_I.Height/2.0f)-68&&
						mousePos.y < (optButtonTex_Set.transMat._42 + optSelectTex_I.Height/2.0f)-68)
			{
				if (buttonSelect != OPTIONS)
				{
					int result = system->playSound(FMOD_CHANNEL_FREE, sound3, false, &myChannel2);
					ERRCHECK(result);
					result = myChannel2->setVolume(0.5);
					ERRCHECK(result); // displays message if this returned an error
				}
				buttonSelect = OPTIONS;
				mouseOff = false;
			}
			else if (	mousePos.x > (quitButtonTex_Set.transMat._41 - quitSelectTex_I.Width/2.0f)-20 &&
						mousePos.x < (quitButtonTex_Set.transMat._41 + quitSelectTex_I.Width/2.0f)-15 &&
						mousePos.y > (quitButtonTex_Set.transMat._42 - quitSelectTex_I.Height/2.0f)-74&&
						mousePos.y < (quitButtonTex_Set.transMat._42 + quitSelectTex_I.Height/2.0f)-74)
			{
				if (buttonSelect != QUITBUTTON)
				{
					int result = system->playSound(FMOD_CHANNEL_FREE, sound3, false, &myChannel2);
					ERRCHECK(result);
					result = myChannel2->setVolume(0.5);
					ERRCHECK(result); // displays message if this returned an error
				}
				buttonSelect = QUITBUTTON;
				mouseOff = false;
			}

			if (mouseOff && keyboard_delay == 0)
			{
				if (buffer[DIK_UP] & 0x80 || buffer[DIK_LEFTARROW] & 0x80)
				{
					keyboard_delay = 10;
					if (buttonSelect <= PLAY)
						buttonSelect = QUITBUTTON;
					else
						buttonSelect--;
					int result = system->playSound(FMOD_CHANNEL_FREE, sound3, false, &myChannel2);
					ERRCHECK(result);
					result = myChannel2->setVolume(0.5);
					ERRCHECK(result); // displays message if this returned an error
				}
				if (buffer[DIK_DOWN] & 0x80 || buffer[DIK_RIGHTARROW] & 0x80)
				{
					keyboard_delay = 10;
					if (buttonSelect == QUITBUTTON ||
						buttonSelect == MAIN)
						buttonSelect = PLAY;
					else
						buttonSelect++;
					int result = system->playSound(FMOD_CHANNEL_FREE, sound3, false, &myChannel2);
					ERRCHECK(result);
					result = myChannel2->setVolume(0.5);
					ERRCHECK(result); // displays message if this returned an error
				}
			}

			if ((!mouseOff && mouseState.rgbButtons[0] & 0x80)||
				(buffer[DIK_RETURN] & 0x80 && keyboard_delay == 0))
			{
				if(buttonSelect == PLAY)
				{
					menuState = PLAY;
					keyboard_delay = 10;
					int result = system->playSound(FMOD_CHANNEL_FREE, sound5, false, &myChannel2);
					ERRCHECK(result);
					result = myChannel2->setVolume(0.5);
					ERRCHECK(result); // displays message if this returned an error
				}
				if(buttonSelect == OPTIONS)
				{
					menuState = OPTIONS;
					buttonSelect = MAIN;
					keyboard_delay = 10;
					int result = system->playSound(FMOD_CHANNEL_FREE, sound5, false, &myChannel2);
					ERRCHECK(result);
					result = myChannel2->setVolume(0.5);
					ERRCHECK(result); // displays message if this returned an error
				}
				if(buttonSelect == CREDITS)
				{
					menuState = CREDITS;
					buttonSelect = MAIN;
					keyboard_delay = 10;
					int result = system->playSound(FMOD_CHANNEL_FREE, sound5, false, &myChannel2);
					ERRCHECK(result);
					result = myChannel2->setVolume(0.5);
					ERRCHECK(result); // displays message if this returned an error
				}
				if(buttonSelect == QUITBUTTON)
				{
					menuState = QUITBUTTON;
					keyboard_delay = 10;
					int result = system->playSound(FMOD_CHANNEL_FREE, sound5, false, &myChannel2);
					ERRCHECK(result);
					result = myChannel2->setVolume(0.5);
					ERRCHECK(result); // displays message if this returned an error
				}
			}

			if (buttonSelect == PLAY)
			{
				center = D3DXVECTOR3(playSelectTex_I.Width/2.0f,playSelectTex_I.Width/2.0f,0); //set the center of the sprite
				m_pD3DSprite->SetTransform(&(playSelectTex_Set.CreateWorldMat()));
				m_pD3DSprite->Draw(playSelectTex,0,&center,0,
								   D3DCOLOR_XRGB(255,255,255)); //draw the sprite
			}
			else
			{
				center = D3DXVECTOR3(playButtonTex_I.Width/2.0f,playButtonTex_I.Width/2.0f,0); //set the center of the sprite
				m_pD3DSprite->SetTransform(&(playButtonTex_Set.CreateWorldMat()));
				m_pD3DSprite->Draw(playButtonTex,0,&center,0,
								   D3DCOLOR_XRGB(255,255,255)); //draw the sprite
			}

			if (buttonSelect == CREDITS)
			{
				center = D3DXVECTOR3(credSelectTex_I.Width/2.0f,credSelectTex_I.Width/2.0f,0); //set the center of the sprite
				m_pD3DSprite->SetTransform(&(credSelectTex_Set.CreateWorldMat()));
				m_pD3DSprite->Draw(credSelectTex,0,&center,0,
								   D3DCOLOR_XRGB(255,255,255)); //draw the sprite
			}
			else
			{
				center = D3DXVECTOR3(credButtonTex_I.Width/2.0f,credButtonTex_I.Width/2.0f,0); //set the center of the sprite
				m_pD3DSprite->SetTransform(&(credButtonTex_Set.CreateWorldMat()));
				m_pD3DSprite->Draw(credButtonTex,0,&center,0,
								   D3DCOLOR_XRGB(255,255,255)); //draw the sprite
			}

			if (buttonSelect == OPTIONS)
			{
				center = D3DXVECTOR3(optSelectTex_I.Width/2.0f,optSelectTex_I.Width/2.0f,0); //set the center of the sprite
				m_pD3DSprite->SetTransform(&(optSelectTex_Set.CreateWorldMat()));
				m_pD3DSprite->Draw(optSelectTex,0,&center,0,
								   D3DCOLOR_XRGB(255,255,255)); //draw the sprite
			}
			else
			{
				center = D3DXVECTOR3(optButtonTex_I.Width/2.0f,optButtonTex_I.Width/2.0f,0); //set the center of the sprite
				m_pD3DSprite->SetTransform(&(optButtonTex_Set.CreateWorldMat()));
				m_pD3DSprite->Draw(optButtonTex,0,&center,0,
								   D3DCOLOR_XRGB(255,255,255)); //draw the sprite
			}

			if (buttonSelect == QUITBUTTON)
			{
				center = D3DXVECTOR3(quitSelectTex_I.Width/2.0f,quitSelectTex_I.Width/2.0f,0); //set the center of the sprite
				m_pD3DSprite->SetTransform(&(quitSelectTex_Set.CreateWorldMat()));
				m_pD3DSprite->Draw(quitSelectTex,0,&center,0,
								   D3DCOLOR_XRGB(255,255,255)); //draw the sprite
			}
			else
			{
				center = D3DXVECTOR3(quitButtonTex_I.Width/2.0f,quitButtonTex_I.Width/2.0f,0); //set the center of the sprite
				m_pD3DSprite->SetTransform(&(quitButtonTex_Set.CreateWorldMat()));
				m_pD3DSprite->Draw(quitButtonTex,0,&center,0,
								   D3DCOLOR_XRGB(255,255,255)); //draw the sprite
			}
		}
		else if (menuState == PLAY)
		{
			gameState = GAME;
			menuState = MAIN;
			buttonSelect = MAIN;
			levelState = LOAD1;
			levelNum = 1;
			myChannel->stop();
			FMOD_RESULT result;
			result = system->playSound(FMOD_CHANNEL_FREE, gameStream,
                           false, &myChannel);
			ERRCHECK(result);

			backButtonTex_Set.transMat._41 = 150;
			backButtonTex_Set.transMat._42 = 100;
			backSelectTex_Set.transMat._41 = 150;
			backSelectTex_Set.transMat._42 = 100;
		}
		else if (menuState == CREDITS)
		{
			if (mousePos.x > (backButtonTex_Set.transMat._41 - backButtonTex_I.Width/2.0f)-10 &&
				mousePos.x < (backButtonTex_Set.transMat._41 + backButtonTex_I.Width/2.0f)-5 &&
				mousePos.y > (backButtonTex_Set.transMat._42 - backButtonTex_I.Height/2.0f)-50&&
				mousePos.y < (backButtonTex_Set.transMat._42 + backButtonTex_I.Height/2.0f)-50)
			{
				if (buttonSelect != PLAY)
				{
					int result = system->playSound(FMOD_CHANNEL_FREE, sound3, false, &myChannel2);
					ERRCHECK(result);
					result = myChannel2->setVolume(0.5);
					ERRCHECK(result); // displays message if this returned an error
				}
				buttonSelect = PLAY;
				mouseOff = false;
			}
			if (mouseOff && keyboard_delay == 0)
			{
				if (buffer[DIK_UP] & 0x80 || buffer[DIK_DOWN] & 0x80 ||
					buffer[DIK_RIGHTARROW] & 0x80 || buffer[DIK_LEFTARROW] & 0x80)
				{
					keyboard_delay = 10;
					buttonSelect = PLAY;
					int result = system->playSound(FMOD_CHANNEL_FREE, sound3, false, &myChannel2);
					ERRCHECK(result);
					result = myChannel2->setVolume(0.5);
					ERRCHECK(result); // displays message if this returned an error
				}
			}
			if ((!mouseOff && mouseState.rgbButtons[0] & 0x80)||
				buffer[DIK_RETURN] & 0x80)
			{
				if(buttonSelect == PLAY)
				{
					menuState = MAIN;
					buttonSelect = MAIN;
					keyboard_delay = 10;
					int result = system->playSound(FMOD_CHANNEL_FREE, sound5, false, &myChannel2);
					ERRCHECK(result);
					result = myChannel2->setVolume(0.5);
					ERRCHECK(result); // displays message if this returned an error
				}
			}
			if (buttonSelect == PLAY)
			{
				center = D3DXVECTOR3(backSelectTex_I.Width/2.0f,backSelectTex_I.Width/2.0f,0); //set the center of the sprite
				m_pD3DSprite->SetTransform(&(backSelectTex_Set.CreateWorldMat()));
				m_pD3DSprite->Draw(backSelectTex,0,&center,0,
								   D3DCOLOR_XRGB(255,255,255)); //draw the sprite
			}
			else
			{
				center = D3DXVECTOR3(backButtonTex_I.Width/2.0f,backButtonTex_I.Width/2.0f,0); //set the center of the sprite
				m_pD3DSprite->SetTransform(&(backButtonTex_Set.CreateWorldMat()));
				m_pD3DSprite->Draw(backButtonTex,0,&center,0,
								   D3DCOLOR_XRGB(255,255,255)); //draw the sprite
			}
			center = D3DXVECTOR3(creditsTex_I.Width/2.0f,creditsTex_I.Width/2.0f,0); //set the center of the sprite
			m_pD3DSprite->SetTransform(&(creditsTex_Set.CreateWorldMat()));
			m_pD3DSprite->Draw(creditsTex,0,&center,0,
								D3DCOLOR_XRGB(255,255,255)); //draw the sprite
		}
		else if (menuState == OPTIONS)
		{
			if (mousePos.x > (backButtonTex_Set.transMat._41 - backButtonTex_I.Width/2.0f)-10 &&
				mousePos.x < (backButtonTex_Set.transMat._41 + backButtonTex_I.Width/2.0f)-5 &&
				mousePos.y > (backButtonTex_Set.transMat._42 - backButtonTex_I.Height/2.0f)-50&&
				mousePos.y < (backButtonTex_Set.transMat._42 + backButtonTex_I.Height/2.0f)-50)
			{
				if (buttonSelect != PLAY)
				{
					int result = system->playSound(FMOD_CHANNEL_FREE, sound3, false, &myChannel2);
					ERRCHECK(result);
					result = myChannel2->setVolume(0.5);
					ERRCHECK(result); // displays message if this returned an error
				}
				buttonSelect = PLAY;
				mouseOff = false;
			}
			else if (	mousePos.x > (fScreenButtonTex_Set.transMat._41 - fScreenButtonTex_I.Width/2.0f)-10 &&
						mousePos.x < (fScreenButtonTex_Set.transMat._41 + fScreenButtonTex_I.Width/2.0f)-7 &&
						mousePos.y > (fScreenButtonTex_Set.transMat._42 - fScreenButtonTex_I.Height/2.0f)-64&&
						mousePos.y < (fScreenButtonTex_Set.transMat._42 + fScreenButtonTex_I.Height/2.0f)-64)
			{
				if (buttonSelect != CREDITS)
				{
					int result = system->playSound(FMOD_CHANNEL_FREE, sound3, false, &myChannel2);
					ERRCHECK(result);
					result = myChannel2->setVolume(0.5);
					ERRCHECK(result); // displays message if this returned an error
				}
				buttonSelect = CREDITS;
				mouseOff = false;
			}
			else if (	mousePos.x > (windButtonTex_Set.transMat._41 - windButtonTex_I.Width/2.0f)-7 &&
						mousePos.x < (windButtonTex_Set.transMat._41 + windButtonTex_I.Width/2.0f)-10 &&
						mousePos.y > (windButtonTex_Set.transMat._42 - windButtonTex_I.Height/2.0f)-64&&
						mousePos.y < (windButtonTex_Set.transMat._42 + windButtonTex_I.Height/2.0f)-64)
			{
				if (buttonSelect != OPTIONS)
				{
					int result = system->playSound(FMOD_CHANNEL_FREE, sound3, false, &myChannel2);
					ERRCHECK(result);
					result = myChannel2->setVolume(0.5);
					ERRCHECK(result); // displays message if this returned an error
				}
				buttonSelect = OPTIONS;
				mouseOff = false;
			}
			if (mouseOff && keyboard_delay == 0)
			{
				if (buffer[DIK_UP] & 0x80 || buffer[DIK_LEFTARROW] & 0x80)
				{
					keyboard_delay = 10;
					if (buttonSelect <= PLAY)
						buttonSelect = OPTIONS;
					else
						buttonSelect--;
					int result = system->playSound(FMOD_CHANNEL_FREE, sound3, false, &myChannel2);
					ERRCHECK(result);
					result = myChannel2->setVolume(0.5);
					ERRCHECK(result); // displays message if this returned an error
				}
				if (buffer[DIK_DOWN] & 0x80 || buffer[DIK_RIGHTARROW] & 0x80)
				{
					keyboard_delay = 10;
					if (buttonSelect == QUITBUTTON ||
						buttonSelect == MAIN)
						buttonSelect = PLAY;
					else
						buttonSelect++;
					int result = system->playSound(FMOD_CHANNEL_FREE, sound3, false, &myChannel2);
					ERRCHECK(result);
					result = myChannel2->setVolume(0.5);
					ERRCHECK(result); // displays message if this returned an error
				}
			}
			if ((!mouseOff && mouseState.rgbButtons[0] & 0x80)||
				buffer[DIK_RETURN] & 0x80)
			{
				if(buttonSelect == PLAY)
				{
					menuState = MAIN;
					buttonSelect = MAIN;
					keyboard_delay = 10;
					int result = system->playSound(FMOD_CHANNEL_FREE, sound5, false, &myChannel2);
					ERRCHECK(result);
					result = myChannel2->setVolume(0.5);
					ERRCHECK(result); // displays message if this returned an error
				}
				if(buttonSelect == CREDITS)
				{
					buttonSelect = MAIN;
					keyboard_delay = 10;
					int result = system->playSound(FMOD_CHANNEL_FREE, sound5, false, &myChannel2);
					ERRCHECK(result);
					result = myChannel2->setVolume(0.5);
					ERRCHECK(result); // displays message if this returned an error

					// Are we already in full screen mode?
					if( D3Dpp.Windowed )
					{
						// Get the current screen resolution.
						int width  = GetSystemMetrics(SM_CXSCREEN);
						int height = GetSystemMetrics(SM_CYSCREEN);
						D3Dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
						D3Dpp.BackBufferWidth  = width;
						D3Dpp.BackBufferHeight = height;
						D3Dpp.Windowed         = false;
						// Change the window style to a more full screen
						// friendly style.
						SetWindowLongPtr(m_hWnd, GWL_STYLE, WS_POPUP);
						// If we call SetWindowLongPtr, MSDN states that we need
						// to call SetWindowPos for the change to take effect.
						// In addition, we need to call this function anyway
						// to update the window dimensions.
						SetWindowPos(m_hWnd, HWND_TOP, 0, 0,
								width, height, SWP_NOZORDER | SWP_SHOWWINDOW);

						// Reset the device with the changes.
						m_pD3DSprite->OnLostDevice();
						m_pD3DDevice->Reset(&D3Dpp);
						m_pD3DSprite->OnResetDevice();
					}
				}
				if(buttonSelect == OPTIONS)
				{
					buttonSelect = MAIN;
					keyboard_delay = 10;
					int result = system->playSound(FMOD_CHANNEL_FREE, sound5, false, &myChannel2);
					ERRCHECK(result);
					result = myChannel2->setVolume(0.5);
					ERRCHECK(result); // displays message if this returned an error

					// Are we already in windowed mode?
					if( !D3Dpp.Windowed )
					{
						// Default to a client rectangle of 800x600.
						RECT R = {0, 0, 800, 600};
						AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
						D3Dpp.BackBufferFormat = D3DFMT_UNKNOWN;
						D3Dpp.BackBufferWidth  = 800;
						D3Dpp.BackBufferHeight = 600;
						D3Dpp.Windowed         = true;
						// Change the window style to a more windowed
						// friendly style.
						SetWindowLongPtr(m_hWnd,GWL_STYLE,WS_OVERLAPPEDWINDOW);
						// If we call SetWindowLongPtr, MSDN states that we
						// need to call SetWindowPos for the change to take effect.
						// In addition, we need to call this function anyway to
						// update the window dimensions.
						SetWindowPos(m_hWnd, HWND_TOP, 100, 100,
								R.right, R.bottom, SWP_NOZORDER | SWP_SHOWWINDOW);

						// Reset the device with the changes.
						m_pD3DSprite->OnLostDevice();
						m_pD3DDevice->Reset(&D3Dpp);
						m_pD3DSprite->OnResetDevice();
					}
				}
			}
			if (buttonSelect == PLAY)
			{
				center = D3DXVECTOR3(backSelectTex_I.Width/2.0f,backSelectTex_I.Width/2.0f,0); //set the center of the sprite
				m_pD3DSprite->SetTransform(&(backSelectTex_Set.CreateWorldMat()));
				m_pD3DSprite->Draw(backSelectTex,0,&center,0,
								   D3DCOLOR_XRGB(255,255,255)); //draw the sprite
			}
			else
			{
				center = D3DXVECTOR3(backButtonTex_I.Width/2.0f,backButtonTex_I.Width/2.0f,0); //set the center of the sprite
				m_pD3DSprite->SetTransform(&(backButtonTex_Set.CreateWorldMat()));
				m_pD3DSprite->Draw(backButtonTex,0,&center,0,
								   D3DCOLOR_XRGB(255,255,255)); //draw the sprite
			}
			if (buttonSelect == CREDITS)
			{
				center = D3DXVECTOR3(fScreenSelectTex_I.Width/2.0f,fScreenSelectTex_I.Width/2.0f,0); //set the center of the sprite
				m_pD3DSprite->SetTransform(&(fScreenSelectTex_Set.CreateWorldMat()));
				m_pD3DSprite->Draw(fScreenSelectTex,0,&center,0,
								   D3DCOLOR_XRGB(255,255,255)); //draw the sprite
			}
			else
			{
				center = D3DXVECTOR3(fScreenButtonTex_I.Width/2.0f,fScreenButtonTex_I.Width/2.0f,0); //set the center of the sprite
				m_pD3DSprite->SetTransform(&(fScreenButtonTex_Set.CreateWorldMat()));
				m_pD3DSprite->Draw(fScreenButtonTex,0,&center,0,
								   D3DCOLOR_XRGB(255,255,255)); //draw the sprite
			}
			if (buttonSelect == OPTIONS)
			{
				center = D3DXVECTOR3(windSelectTex_I.Width/2.0f,windSelectTex_I.Width/2.0f,0); //set the center of the sprite
				m_pD3DSprite->SetTransform(&(windSelectTex_Set.CreateWorldMat()));
				m_pD3DSprite->Draw(windSelectTex,0,&center,0,
								   D3DCOLOR_XRGB(255,255,255)); //draw the sprite
			}
			else
			{
				center = D3DXVECTOR3(windButtonTex_I.Width/2.0f,windButtonTex_I.Width/2.0f,0); //set the center of the sprite
				m_pD3DSprite->SetTransform(&(windButtonTex_Set.CreateWorldMat()));
				m_pD3DSprite->Draw(windButtonTex,0,&center,0,
								   D3DCOLOR_XRGB(255,255,255)); //draw the sprite
			}
		}
		if (menuState == QUITBUTTON)
		{
			quit = true;
		}
		m_pD3DSprite->End();

		m_pD3DDevice->EndScene();
		m_pD3DDevice->Present(0,0,0,0);
		
	//*************************************************************************
	}
	if (gameState == GAME)
	{
		if (levelState == LOAD1)
		{
			gameWin = false;
			for(int i = 0; i < 10; i++)
			{
				for(int j = 0; j < 10; j++)
					grid[i][j] = 0;
			}
			for(int j = 0; j < 3; j++)
			{
				for(int i = 0; i < 10; i++)
					grid[i][j] = -1;
			}
			for(int j = 6; j < 10; j++)
			{
				for(int i = 0; i < 10; i++)
					grid[i][j] = -1;
			}
			for(int i = 0; i < 2; i++)
			{
				for(int j = 0; j < 10; j++)
					grid[i][j] = -1;
			}
			for(int i = 8; i < 10; i++)
			{
				for(int j = 0; j < 10; j++)
					grid[i][j] = -1;
			}
			playerPos[X] = 3;
			playerPos[Y] = 5;
			grid[playerPos[X]-1][playerPos[Y]-1] = 1;
			playerDying = false;
			levelChanging = false;
			aI1Pos[X] = 8;
			aI1Pos[Y] = 5;
			grid[aI1Pos[X]-1][aI1Pos[Y]-1] = 2;

			player = Player(Vector3D(grid_size * playerPos[X] - grid_size/2.0,
								 grid_size * playerPos[Y] - grid_size/2.0, 0),
								 Vector3D(800,600,0),grid_size,m_pD3DDevice);
			ai_1 = AIUnit(Vector3D(grid_size * aI1Pos[X] - grid_size/2.0,
								 grid_size * aI1Pos[Y] - grid_size/2.0, 0),
								 Vector3D(800,600,0),grid_size,m_pD3DDevice,1);
			ai_2 = AIUnit();
			ai_3 = AIUnit();
			ai_4 = AIUnit();
			ai_5 = AIUnit();
			ai_6 = AIUnit();
			ai_7 = AIUnit();
			ai_8 = AIUnit();
			ai_9 = AIUnit();
			ai_10 = AIUnit();
			levelState = LEVELONE;
		}
		if (levelState == LEVELONE)
		{
			// Clear the back buffer, call BeginScene()
			m_pD3DDevice->Clear(0,0,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(255,255,255), 1.0f, 0);
			m_pD3DDevice->BeginScene();

			m_pD3DSprite->Begin(D3DXSPRITE_ALPHABLEND);

			D3DXVECTOR3 center(0,0,0); //set the center of the sprite
			m_pD3DSprite->SetTransform(&(menuBackgroundSetup.CreateWorldMat()));
			m_pD3DSprite->Draw(menuBackground,0,&center,0,
							   D3DCOLOR_XRGB(255,255,255)); //draw the sprite

			POINT mousePos;
			bool mouseOff = true;
		
			GetCursorPos(&mousePos);
			ScreenToClient(m_hWnd,&mousePos);

			if (	mousePos.x > (mainButtonTex_Set.transMat._41 - mainSelectTex_I.Width/2.0f)-20 &&
					mousePos.x < (mainButtonTex_Set.transMat._41 + mainSelectTex_I.Width/2.0f)-15 &&
					mousePos.y > (mainButtonTex_Set.transMat._42 - mainSelectTex_I.Height/2.0f)-64&&
					mousePos.y < (mainButtonTex_Set.transMat._42 + mainSelectTex_I.Height/2.0f)-64)
			{
				if (buttonSelect != PLAY)
				{
					int result = system->playSound(FMOD_CHANNEL_FREE, sound3, false, &myChannel2);
					ERRCHECK(result);
					result = myChannel2->setVolume(0.5);
					ERRCHECK(result); // displays message if this returned an error
				}
				buttonSelect = PLAY;
				mouseOff = false;
			}
			else if (	mousePos.x > (restartButtonTex_Set.transMat._41 - restartSelectTex_I.Width/2.0f)-20 &&
						mousePos.x < (restartButtonTex_Set.transMat._41 + restartSelectTex_I.Width/2.0f)-15 &&
						mousePos.y > (restartButtonTex_Set.transMat._42 - restartSelectTex_I.Height/2.0f)-68&&
						mousePos.y < (restartButtonTex_Set.transMat._42 + restartSelectTex_I.Height/2.0f)-68)
			{
				if (buttonSelect != CREDITS)
				{
					int result = system->playSound(FMOD_CHANNEL_FREE, sound3, false, &myChannel2);
					ERRCHECK(result);
					result = myChannel2->setVolume(0.5);
					ERRCHECK(result); // displays message if this returned an error
				}
				buttonSelect = CREDITS;
				mouseOff = false;
			}
			else
				buttonSelect = MAIN;

			if (!mouseOff && mouseState.rgbButtons[0] & 0x80 && keyboard_delay == 0)
			{
				if(buttonSelect == PLAY)
				{
					gameState = MENU;
					buttonSelect = MAIN;
					myChannel->stop();
					FMOD_RESULT result;
					result = system->playSound(FMOD_CHANNEL_FREE, myStream,
								   false, &myChannel);
					ERRCHECK(result);
					result = system->playSound(FMOD_CHANNEL_FREE, sound5, false, &myChannel2);
					ERRCHECK(result);
					result = myChannel2->setVolume(0.5);
					ERRCHECK(result); // displays message if this returned an error
					keyboard_delay = 10.0;
				}
				if(buttonSelect == CREDITS)
				{
					levelState = LOAD1;
					int result = system->playSound(FMOD_CHANNEL_FREE, sound5, false, &myChannel2);
					ERRCHECK(result);
					result = myChannel2->setVolume(0.5);
					ERRCHECK(result); // displays message if this returned an error
					keyboard_delay = 10.0;
				}
			}
			if (buffer[DIK_R] & 0x80 && keyboard_delay == 0)
			{
				levelState = LOAD1;
				int result = system->playSound(FMOD_CHANNEL_FREE, sound5, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				keyboard_delay = 10.0;
			}

			if (buttonSelect == PLAY)
			{
				center = D3DXVECTOR3(mainSelectTex_I.Width/2.0f,mainSelectTex_I.Width/2.0f,0); //set the center of the sprite
				m_pD3DSprite->SetTransform(&(mainSelectTex_Set.CreateWorldMat()));
				m_pD3DSprite->Draw(mainSelectTex,0,&center,0,
								   D3DCOLOR_XRGB(255,255,255)); //draw the sprite
			}
			else
			{
				center = D3DXVECTOR3(mainButtonTex_I.Width/2.0f,mainButtonTex_I.Width/2.0f,0); //set the center of the sprite
				m_pD3DSprite->SetTransform(&(mainButtonTex_Set.CreateWorldMat()));
				m_pD3DSprite->Draw(mainButtonTex,0,&center,0,
								   D3DCOLOR_XRGB(255,255,255)); //draw the sprite
			}

			if (buttonSelect == CREDITS)
			{
				center = D3DXVECTOR3(restartSelectTex_I.Width/2.0f,restartSelectTex_I.Width/2.0f,0); //set the center of the sprite
				m_pD3DSprite->SetTransform(&(restartSelectTex_Set.CreateWorldMat()));
				m_pD3DSprite->Draw(restartSelectTex,0,&center,0,
								   D3DCOLOR_XRGB(255,255,255)); //draw the sprite
			}
			else
			{
				center = D3DXVECTOR3(restartButtonTex_I.Width/2.0f,restartButtonTex_I.Width/2.0f,0); //set the center of the sprite
				m_pD3DSprite->SetTransform(&(restartButtonTex_Set.CreateWorldMat()));
				m_pD3DSprite->Draw(restartButtonTex,0,&center,0,
								   D3DCOLOR_XRGB(255,255,255)); //draw the sprite
			}

			center = D3DXVECTOR3(LevelOneTex_I.Width/2.0f,LevelOneTex_I.Width/2.0f,0); //set the center of the sprite
			m_pD3DSprite->SetTransform(&(LevelOneTex_Set.CreateWorldMat()));
			m_pD3DSprite->Draw(LevelOneTex,0,&center,0,
								D3DCOLOR_XRGB(255,255,255)); //draw the sprite

			DisplayGrid();

			DisplayCharacters();

			m_pD3DSprite->End();

			m_pD3DDevice->EndScene();
			m_pD3DDevice->Present(0,0,0,0);
		}
		if (levelState == LOAD2)
		{
			gameWin = false;
			for(int i = 0; i < 10; i++)
			{
				for(int j = 0; j < 10; j++)
					grid[i][j] = 0;
			}
			for(int j = 0; j < 3; j++)
			{
				for(int i = 0; i < 10; i++)
					grid[i][j] = -1;
			}
			for(int j = 6; j < 10; j++)
			{
				for(int i = 0; i < 10; i++)
					grid[i][j] = -1;
			}
			for(int i = 0; i < 2; i++)
			{
				for(int j = 0; j < 10; j++)
					grid[i][j] = -1;
			}
			for(int i = 8; i < 10; i++)
			{
				for(int j = 0; j < 10; j++)
					grid[i][j] = -1;
			}
			playerPos[X] = 6;
			playerPos[Y] = 5;
			grid[playerPos[X]-1][playerPos[Y]-1] = 1;
			playerDying = false;
			levelChanging = false;
			aI1Pos[X] = 8;
			aI1Pos[Y] = 5;
			grid[aI1Pos[X]-1][aI1Pos[Y]-1] = 2;
			aI2Pos[X] = 3;
			aI2Pos[Y] = 5;
			grid[aI2Pos[X]-1][aI2Pos[Y]-1] = 2;

			player = Player(Vector3D(grid_size * playerPos[X] - grid_size/2.0,
								 grid_size * playerPos[Y] - grid_size/2.0, 0),
								 Vector3D(800,600,0),grid_size,m_pD3DDevice);
			ai_1 = AIUnit(Vector3D(grid_size * aI1Pos[X] - grid_size/2.0,
								 grid_size * aI1Pos[Y] - grid_size/2.0, 0),
								 Vector3D(800,600,0),grid_size,m_pD3DDevice,2);
			ai_2 = AIUnit(Vector3D(grid_size * aI2Pos[X] - grid_size/2.0,
								 grid_size * aI2Pos[Y] - grid_size/2.0, 0),
								 Vector3D(800,600,0),grid_size,m_pD3DDevice,2);
			ai_3 = AIUnit();
			ai_4 = AIUnit();
			ai_5 = AIUnit();
			ai_6 = AIUnit();
			ai_7 = AIUnit();
			ai_8 = AIUnit();
			ai_9 = AIUnit();
			ai_10 = AIUnit();
			levelState = LEVELTWO;
		}
		if (levelState == LEVELTWO)
		{
			// Clear the back buffer, call BeginScene()
			m_pD3DDevice->Clear(0,0,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(255,255,255), 1.0f, 0);
			m_pD3DDevice->BeginScene();

			m_pD3DSprite->Begin(D3DXSPRITE_ALPHABLEND);

			D3DXVECTOR3 center(0,0,0); //set the center of the sprite
			m_pD3DSprite->SetTransform(&(menuBackgroundSetup.CreateWorldMat()));
			m_pD3DSprite->Draw(menuBackground,0,&center,0,
							   D3DCOLOR_XRGB(255,255,255)); //draw the sprite

			POINT mousePos;
			bool mouseOff = true;
		
			GetCursorPos(&mousePos);
			ScreenToClient(m_hWnd,&mousePos);

			if (	mousePos.x > (mainButtonTex_Set.transMat._41 - mainSelectTex_I.Width/2.0f)-20 &&
					mousePos.x < (mainButtonTex_Set.transMat._41 + mainSelectTex_I.Width/2.0f)-15 &&
					mousePos.y > (mainButtonTex_Set.transMat._42 - mainSelectTex_I.Height/2.0f)-64&&
					mousePos.y < (mainButtonTex_Set.transMat._42 + mainSelectTex_I.Height/2.0f)-64)
			{
				if (buttonSelect != PLAY)
				{
					int result = system->playSound(FMOD_CHANNEL_FREE, sound3, false, &myChannel2);
					ERRCHECK(result);
					result = myChannel2->setVolume(0.5);
					ERRCHECK(result); // displays message if this returned an error
				}
				buttonSelect = PLAY;
				mouseOff = false;
			}
			else if (	mousePos.x > (restartButtonTex_Set.transMat._41 - restartSelectTex_I.Width/2.0f)-20 &&
						mousePos.x < (restartButtonTex_Set.transMat._41 + restartSelectTex_I.Width/2.0f)-15 &&
						mousePos.y > (restartButtonTex_Set.transMat._42 - restartSelectTex_I.Height/2.0f)-68&&
						mousePos.y < (restartButtonTex_Set.transMat._42 + restartSelectTex_I.Height/2.0f)-68)
			{
				if (buttonSelect != CREDITS)
				{
					int result = system->playSound(FMOD_CHANNEL_FREE, sound3, false, &myChannel2);
					ERRCHECK(result);
					result = myChannel2->setVolume(0.5);
					ERRCHECK(result); // displays message if this returned an error
				}
				buttonSelect = CREDITS;
				mouseOff = false;
			}
			else
				buttonSelect = MAIN;

			if (!mouseOff && mouseState.rgbButtons[0] & 0x80 && keyboard_delay == 0)
			{
				if(buttonSelect == PLAY)
				{
					gameState = MENU;
					buttonSelect = MAIN;
					myChannel->stop();
					FMOD_RESULT result;
					result = system->playSound(FMOD_CHANNEL_FREE, myStream,
								   false, &myChannel);
					ERRCHECK(result);
					result = system->playSound(FMOD_CHANNEL_FREE, sound5, false, &myChannel2);
					ERRCHECK(result);
					result = myChannel2->setVolume(0.5);
					ERRCHECK(result); // displays message if this returned an error
					keyboard_delay = 10.0;
				}
				if(buttonSelect == CREDITS)
				{
					levelState = LOAD2;
					int result = system->playSound(FMOD_CHANNEL_FREE, sound5, false, &myChannel2);
					ERRCHECK(result);
					result = myChannel2->setVolume(0.5);
					ERRCHECK(result); // displays message if this returned an error
					keyboard_delay = 10.0;
				}
			}
			if (buffer[DIK_R] & 0x80 && keyboard_delay == 0)
			{
				levelState = LOAD2;
				int result = system->playSound(FMOD_CHANNEL_FREE, sound5, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				keyboard_delay = 10.0;
			}

			if (buttonSelect == PLAY)
			{
				center = D3DXVECTOR3(mainSelectTex_I.Width/2.0f,mainSelectTex_I.Width/2.0f,0); //set the center of the sprite
				m_pD3DSprite->SetTransform(&(mainSelectTex_Set.CreateWorldMat()));
				m_pD3DSprite->Draw(mainSelectTex,0,&center,0,
								   D3DCOLOR_XRGB(255,255,255)); //draw the sprite
			}
			else
			{
				center = D3DXVECTOR3(mainButtonTex_I.Width/2.0f,mainButtonTex_I.Width/2.0f,0); //set the center of the sprite
				m_pD3DSprite->SetTransform(&(mainButtonTex_Set.CreateWorldMat()));
				m_pD3DSprite->Draw(mainButtonTex,0,&center,0,
								   D3DCOLOR_XRGB(255,255,255)); //draw the sprite
			}

			if (buttonSelect == CREDITS)
			{
				center = D3DXVECTOR3(restartSelectTex_I.Width/2.0f,restartSelectTex_I.Width/2.0f,0); //set the center of the sprite
				m_pD3DSprite->SetTransform(&(restartSelectTex_Set.CreateWorldMat()));
				m_pD3DSprite->Draw(restartSelectTex,0,&center,0,
								   D3DCOLOR_XRGB(255,255,255)); //draw the sprite
			}
			else
			{
				center = D3DXVECTOR3(restartButtonTex_I.Width/2.0f,restartButtonTex_I.Width/2.0f,0); //set the center of the sprite
				m_pD3DSprite->SetTransform(&(restartButtonTex_Set.CreateWorldMat()));
				m_pD3DSprite->Draw(restartButtonTex,0,&center,0,
								   D3DCOLOR_XRGB(255,255,255)); //draw the sprite
			}

			center = D3DXVECTOR3(LevelTwoTex_I.Width/2.0f,LevelTwoTex_I.Width/2.0f,0); //set the center of the sprite
			m_pD3DSprite->SetTransform(&(LevelTwoTex_Set.CreateWorldMat()));
			m_pD3DSprite->Draw(LevelTwoTex,0,&center,0,
								D3DCOLOR_XRGB(255,255,255)); //draw the sprite

			DisplayGrid();

			DisplayCharacters();

			m_pD3DSprite->End();

			m_pD3DDevice->EndScene();
			m_pD3DDevice->Present(0,0,0,0);
		}
		if (levelState == LOAD3)
		{
			gameWin = false;
			for(int i = 0; i < 10; i++)
			{
				for(int j = 0; j < 10; j++)
					grid[i][j] = 0;
			}
			for(int j = 0; j < 2; j++)
			{
				for(int i = 0; i < 10; i++)
					grid[i][j] = -1;
			}
			for(int j = 7; j < 10; j++)
			{
				for(int i = 0; i < 10; i++)
					grid[i][j] = -1;
			}
			for(int i = 0; i < 2; i++)
			{
				for(int j = 0; j < 10; j++)
					grid[i][j] = -1;
			}
			for(int i = 9; i < 10; i++)
			{
				for(int j = 0; j < 10; j++)
					grid[i][j] = -1;
			}
			playerPos[X] = 6;
			playerPos[Y] = 5;
			grid[playerPos[X]-1][playerPos[Y]-1] = 1;
			playerDying = false;
			levelChanging = false;
			aI1Pos[X] = 9;
			aI1Pos[Y] = 5;
			grid[aI1Pos[X]-1][aI1Pos[Y]-1] = 2;
			aI2Pos[X] = 3;
			aI2Pos[Y] = 5;
			grid[aI2Pos[X]-1][aI2Pos[Y]-1] = 2;
			aI3Pos[X] = 6;
			aI3Pos[Y] = 3;
			grid[aI3Pos[X]-1][aI3Pos[Y]-1] = 2;
			aI4Pos[X] = 6;
			aI4Pos[Y] = 7;
			grid[aI4Pos[X]-1][aI4Pos[Y]-1] = 2;

			player = Player(Vector3D(grid_size * playerPos[X] - grid_size/2.0,
								 grid_size * playerPos[Y] - grid_size/2.0, 0),
								 Vector3D(800,600,0),grid_size,m_pD3DDevice);
			ai_1 = AIUnit(Vector3D(grid_size * aI1Pos[X] - grid_size/2.0,
								 grid_size * aI1Pos[Y] - grid_size/2.0, 0),
								 Vector3D(800,600,0),grid_size,m_pD3DDevice,2);
			ai_2 = AIUnit(Vector3D(grid_size * aI2Pos[X] - grid_size/2.0,
								 grid_size * aI2Pos[Y] - grid_size/2.0, 0),
								 Vector3D(800,600,0),grid_size,m_pD3DDevice,2);
			ai_3 = AIUnit(Vector3D(grid_size * aI3Pos[X] - grid_size/2.0,
								 grid_size * aI3Pos[Y] - grid_size/2.0, 0),
								 Vector3D(800,600,0),grid_size,m_pD3DDevice,1);
			ai_4 = AIUnit(Vector3D(grid_size * aI4Pos[X] - grid_size/2.0,
								 grid_size * aI4Pos[Y] - grid_size/2.0, 0),
								 Vector3D(800,600,0),grid_size,m_pD3DDevice,1);
			ai_5 = AIUnit();
			ai_6 = AIUnit();
			ai_7 = AIUnit();
			ai_8 = AIUnit();
			ai_9 = AIUnit();
			ai_10 = AIUnit();
			levelState = LEVELTHREE;
		}
		if (levelState == LEVELTHREE)
		{
			// Clear the back buffer, call BeginScene()
			m_pD3DDevice->Clear(0,0,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(255,255,255), 1.0f, 0);
			m_pD3DDevice->BeginScene();

			m_pD3DSprite->Begin(D3DXSPRITE_ALPHABLEND);

			D3DXVECTOR3 center(0,0,0); //set the center of the sprite
			m_pD3DSprite->SetTransform(&(menuBackgroundSetup.CreateWorldMat()));
			m_pD3DSprite->Draw(menuBackground,0,&center,0,
							   D3DCOLOR_XRGB(255,255,255)); //draw the sprite

			POINT mousePos;
			bool mouseOff = true;
		
			GetCursorPos(&mousePos);
			ScreenToClient(m_hWnd,&mousePos);

			if (	mousePos.x > (mainButtonTex_Set.transMat._41 - mainSelectTex_I.Width/2.0f)-20 &&
					mousePos.x < (mainButtonTex_Set.transMat._41 + mainSelectTex_I.Width/2.0f)-15 &&
					mousePos.y > (mainButtonTex_Set.transMat._42 - mainSelectTex_I.Height/2.0f)-64&&
					mousePos.y < (mainButtonTex_Set.transMat._42 + mainSelectTex_I.Height/2.0f)-64)
			{
				if (buttonSelect != PLAY)
				{
					int result = system->playSound(FMOD_CHANNEL_FREE, sound3, false, &myChannel2);
					ERRCHECK(result);
					result = myChannel2->setVolume(0.5);
					ERRCHECK(result); // displays message if this returned an error
				}
				buttonSelect = PLAY;
				mouseOff = false;
			}
			else if (	mousePos.x > (restartButtonTex_Set.transMat._41 - restartSelectTex_I.Width/2.0f)-20 &&
						mousePos.x < (restartButtonTex_Set.transMat._41 + restartSelectTex_I.Width/2.0f)-15 &&
						mousePos.y > (restartButtonTex_Set.transMat._42 - restartSelectTex_I.Height/2.0f)-68&&
						mousePos.y < (restartButtonTex_Set.transMat._42 + restartSelectTex_I.Height/2.0f)-68)
			{
				if (buttonSelect != CREDITS)
				{
					int result = system->playSound(FMOD_CHANNEL_FREE, sound3, false, &myChannel2);
					ERRCHECK(result);
					result = myChannel2->setVolume(0.5);
					ERRCHECK(result); // displays message if this returned an error
				}
				buttonSelect = CREDITS;
				mouseOff = false;
			}
			else
				buttonSelect = MAIN;

			if (!mouseOff && mouseState.rgbButtons[0] & 0x80 && keyboard_delay == 0)
			{
				if(buttonSelect == PLAY)
				{
					gameState = MENU;
					buttonSelect = MAIN;
					myChannel->stop();
					FMOD_RESULT result;
					result = system->playSound(FMOD_CHANNEL_FREE, myStream,
								   false, &myChannel);
					ERRCHECK(result);
					result = system->playSound(FMOD_CHANNEL_FREE, sound5, false, &myChannel2);
					ERRCHECK(result);
					result = myChannel2->setVolume(0.5);
					ERRCHECK(result); // displays message if this returned an error
					keyboard_delay = 10.0;
				}
				if(buttonSelect == CREDITS)
				{
					levelState = LOAD3;
					int result = system->playSound(FMOD_CHANNEL_FREE, sound5, false, &myChannel2);
					ERRCHECK(result);
					result = myChannel2->setVolume(0.5);
					ERRCHECK(result); // displays message if this returned an error
					keyboard_delay = 10.0;
				}
			}
			if (buffer[DIK_R] & 0x80 && keyboard_delay == 0)
			{
				levelState = LOAD3;
				int result = system->playSound(FMOD_CHANNEL_FREE, sound5, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				keyboard_delay = 10.0;
			}

			if (buttonSelect == PLAY)
			{
				center = D3DXVECTOR3(mainSelectTex_I.Width/2.0f,mainSelectTex_I.Width/2.0f,0); //set the center of the sprite
				m_pD3DSprite->SetTransform(&(mainSelectTex_Set.CreateWorldMat()));
				m_pD3DSprite->Draw(mainSelectTex,0,&center,0,
								   D3DCOLOR_XRGB(255,255,255)); //draw the sprite
			}
			else
			{
				center = D3DXVECTOR3(mainButtonTex_I.Width/2.0f,mainButtonTex_I.Width/2.0f,0); //set the center of the sprite
				m_pD3DSprite->SetTransform(&(mainButtonTex_Set.CreateWorldMat()));
				m_pD3DSprite->Draw(mainButtonTex,0,&center,0,
								   D3DCOLOR_XRGB(255,255,255)); //draw the sprite
			}

			if (buttonSelect == CREDITS)
			{
				center = D3DXVECTOR3(restartSelectTex_I.Width/2.0f,restartSelectTex_I.Width/2.0f,0); //set the center of the sprite
				m_pD3DSprite->SetTransform(&(restartSelectTex_Set.CreateWorldMat()));
				m_pD3DSprite->Draw(restartSelectTex,0,&center,0,
								   D3DCOLOR_XRGB(255,255,255)); //draw the sprite
			}
			else
			{
				center = D3DXVECTOR3(restartButtonTex_I.Width/2.0f,restartButtonTex_I.Width/2.0f,0); //set the center of the sprite
				m_pD3DSprite->SetTransform(&(restartButtonTex_Set.CreateWorldMat()));
				m_pD3DSprite->Draw(restartButtonTex,0,&center,0,
								   D3DCOLOR_XRGB(255,255,255)); //draw the sprite
			}

			center = D3DXVECTOR3(LevelThreeTex_I.Width/2.0f,LevelThreeTex_I.Width/2.0f,0); //set the center of the sprite
			m_pD3DSprite->SetTransform(&(LevelThreeTex_Set.CreateWorldMat()));
			m_pD3DSprite->Draw(LevelThreeTex,0,&center,0,
								D3DCOLOR_XRGB(255,255,255)); //draw the sprite

			DisplayGrid();

			DisplayCharacters();

			m_pD3DSprite->End();

			m_pD3DDevice->EndScene();
			m_pD3DDevice->Present(0,0,0,0);
		}
		if (levelState == LOAD4)
		{
			gameWin = false;
			for(int i = 0; i < 10; i++)
			{
				for(int j = 0; j < 10; j++)
					grid[i][j] = 0;
			}
			for(int j = 0; j < 3; j++)
			{
				for(int i = 0; i < 10; i++)
					grid[i][j] = -1;
			}
			for(int j = 8; j < 10; j++)
			{
				for(int i = 0; i < 10; i++)
					grid[i][j] = -1;
			}
			for(int i = 0; i < 3; i++)
			{
				for(int j = 0; j < 10; j++)
					grid[i][j] = -1;
			}
			for(int i = 8; i < 10; i++)
			{
				for(int j = 0; j < 10; j++)
					grid[i][j] = -1;
			}
			playerPos[X] = 6;
			playerPos[Y] = 6;
			grid[playerPos[X]-1][playerPos[Y]-1] = 1;
			playerDying = false;
			levelChanging = false;
			aI1Pos[X] = 5;
			aI1Pos[Y] = 5;
			grid[aI1Pos[X]-1][aI1Pos[Y]-1] = 2;
			aI2Pos[X] = 7;
			aI2Pos[Y] = 7;
			grid[aI2Pos[X]-1][aI2Pos[Y]-1] = 2;
			aI3Pos[X] = 8;
			aI3Pos[Y] = 6;
			grid[aI3Pos[X]-1][aI3Pos[Y]-1] = 2;
			aI4Pos[X] = 4;
			aI4Pos[Y] = 6;
			grid[aI4Pos[X]-1][aI4Pos[Y]-1] = 2;
			aI5Pos[X] = 8;
			aI5Pos[Y] = 7;
			grid[aI5Pos[X]-1][aI5Pos[Y]-1] = 2;
			aI6Pos[X] = 4;
			aI6Pos[Y] = 5;
			grid[aI6Pos[X]-1][aI6Pos[Y]-1] = 2;

			player = Player(Vector3D(grid_size * playerPos[X] - grid_size/2.0,
								 grid_size * playerPos[Y] - grid_size/2.0, 0),
								 Vector3D(800,600,0),grid_size,m_pD3DDevice);
			ai_1 = AIUnit(Vector3D(grid_size * aI1Pos[X] - grid_size/2.0,
								 grid_size * aI1Pos[Y] - grid_size/2.0, 0),
								 Vector3D(800,600,0),grid_size,m_pD3DDevice,1);
			ai_2 = AIUnit(Vector3D(grid_size * aI2Pos[X] - grid_size/2.0,
								 grid_size * aI2Pos[Y] - grid_size/2.0, 0),
								 Vector3D(800,600,0),grid_size,m_pD3DDevice,1);
			ai_3 = AIUnit(Vector3D(grid_size * aI3Pos[X] - grid_size/2.0,
								 grid_size * aI3Pos[Y] - grid_size/2.0, 0),
								 Vector3D(800,600,0),grid_size,m_pD3DDevice,1);
			ai_4 = AIUnit(Vector3D(grid_size * aI4Pos[X] - grid_size/2.0,
								 grid_size * aI4Pos[Y] - grid_size/2.0, 0),
								 Vector3D(800,600,0),grid_size,m_pD3DDevice,1);
			ai_5 = AIUnit(Vector3D(grid_size * aI5Pos[X] - grid_size/2.0,
								 grid_size * aI5Pos[Y] - grid_size/2.0, 0),
								 Vector3D(800,600,0),grid_size,m_pD3DDevice,1);
			ai_6 = AIUnit(Vector3D(grid_size * aI6Pos[X] - grid_size/2.0,
								 grid_size * aI6Pos[Y] - grid_size/2.0, 0),
								 Vector3D(800,600,0),grid_size,m_pD3DDevice,1);
			ai_7 = AIUnit();
			ai_8 = AIUnit();
			ai_9 = AIUnit();
			ai_10 = AIUnit();
			levelState = LEVELFOUR;
		}
		if (levelState == LEVELFOUR)
		{
			// Clear the back buffer, call BeginScene()
			m_pD3DDevice->Clear(0,0,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(255,255,255), 1.0f, 0);
			m_pD3DDevice->BeginScene();

			m_pD3DSprite->Begin(D3DXSPRITE_ALPHABLEND);

			D3DXVECTOR3 center(0,0,0); //set the center of the sprite
			m_pD3DSprite->SetTransform(&(menuBackgroundSetup.CreateWorldMat()));
			m_pD3DSprite->Draw(menuBackground,0,&center,0,
							   D3DCOLOR_XRGB(255,255,255)); //draw the sprite

			POINT mousePos;
			bool mouseOff = true;
		
			GetCursorPos(&mousePos);
			ScreenToClient(m_hWnd,&mousePos);

			if (	mousePos.x > (mainButtonTex_Set.transMat._41 - mainSelectTex_I.Width/2.0f)-20 &&
					mousePos.x < (mainButtonTex_Set.transMat._41 + mainSelectTex_I.Width/2.0f)-15 &&
					mousePos.y > (mainButtonTex_Set.transMat._42 - mainSelectTex_I.Height/2.0f)-64&&
					mousePos.y < (mainButtonTex_Set.transMat._42 + mainSelectTex_I.Height/2.0f)-64)
			{
				if (buttonSelect != PLAY)
				{
					int result = system->playSound(FMOD_CHANNEL_FREE, sound3, false, &myChannel2);
					ERRCHECK(result);
					result = myChannel2->setVolume(0.5);
					ERRCHECK(result); // displays message if this returned an error
				}
				buttonSelect = PLAY;
				mouseOff = false;
			}
			else if (	mousePos.x > (restartButtonTex_Set.transMat._41 - restartSelectTex_I.Width/2.0f)-20 &&
						mousePos.x < (restartButtonTex_Set.transMat._41 + restartSelectTex_I.Width/2.0f)-15 &&
						mousePos.y > (restartButtonTex_Set.transMat._42 - restartSelectTex_I.Height/2.0f)-68&&
						mousePos.y < (restartButtonTex_Set.transMat._42 + restartSelectTex_I.Height/2.0f)-68)
			{
				if (buttonSelect != CREDITS)
				{
					int result = system->playSound(FMOD_CHANNEL_FREE, sound3, false, &myChannel2);
					ERRCHECK(result);
					result = myChannel2->setVolume(0.5);
					ERRCHECK(result); // displays message if this returned an error
				}
				buttonSelect = CREDITS;
				mouseOff = false;
			}
			else
				buttonSelect = MAIN;

			if (!mouseOff && mouseState.rgbButtons[0] & 0x80 && keyboard_delay == 0)
			{
				if(buttonSelect == PLAY)
				{
					gameState = MENU;
					buttonSelect = MAIN;
					myChannel->stop();
					FMOD_RESULT result;
					result = system->playSound(FMOD_CHANNEL_FREE, myStream,
								   false, &myChannel);
					ERRCHECK(result);
					result = system->playSound(FMOD_CHANNEL_FREE, sound5, false, &myChannel2);
					ERRCHECK(result);
					result = myChannel2->setVolume(0.5);
					ERRCHECK(result); // displays message if this returned an error
					keyboard_delay = 10.0;
				}
				if(buttonSelect == CREDITS)
				{
					levelState = LOAD4;
					int result = system->playSound(FMOD_CHANNEL_FREE, sound5, false, &myChannel2);
					ERRCHECK(result);
					result = myChannel2->setVolume(0.5);
					ERRCHECK(result); // displays message if this returned an error
					keyboard_delay = 10.0;
				}
			}
			if (buffer[DIK_R] & 0x80 && keyboard_delay == 0)
			{
				levelState = LOAD4;
				int result = system->playSound(FMOD_CHANNEL_FREE, sound5, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				keyboard_delay = 10.0;
			}

			if (buttonSelect == PLAY)
			{
				center = D3DXVECTOR3(mainSelectTex_I.Width/2.0f,mainSelectTex_I.Width/2.0f,0); //set the center of the sprite
				m_pD3DSprite->SetTransform(&(mainSelectTex_Set.CreateWorldMat()));
				m_pD3DSprite->Draw(mainSelectTex,0,&center,0,
								   D3DCOLOR_XRGB(255,255,255)); //draw the sprite
			}
			else
			{
				center = D3DXVECTOR3(mainButtonTex_I.Width/2.0f,mainButtonTex_I.Width/2.0f,0); //set the center of the sprite
				m_pD3DSprite->SetTransform(&(mainButtonTex_Set.CreateWorldMat()));
				m_pD3DSprite->Draw(mainButtonTex,0,&center,0,
								   D3DCOLOR_XRGB(255,255,255)); //draw the sprite
			}

			if (buttonSelect == CREDITS)
			{
				center = D3DXVECTOR3(restartSelectTex_I.Width/2.0f,restartSelectTex_I.Width/2.0f,0); //set the center of the sprite
				m_pD3DSprite->SetTransform(&(restartSelectTex_Set.CreateWorldMat()));
				m_pD3DSprite->Draw(restartSelectTex,0,&center,0,
								   D3DCOLOR_XRGB(255,255,255)); //draw the sprite
			}
			else
			{
				center = D3DXVECTOR3(restartButtonTex_I.Width/2.0f,restartButtonTex_I.Width/2.0f,0); //set the center of the sprite
				m_pD3DSprite->SetTransform(&(restartButtonTex_Set.CreateWorldMat()));
				m_pD3DSprite->Draw(restartButtonTex,0,&center,0,
								   D3DCOLOR_XRGB(255,255,255)); //draw the sprite
			}

			center = D3DXVECTOR3(LevelFourTex_I.Width/2.0f,LevelFourTex_I.Width/2.0f,0); //set the center of the sprite
			m_pD3DSprite->SetTransform(&(LevelFourTex_Set.CreateWorldMat()));
			m_pD3DSprite->Draw(LevelFourTex,0,&center,0,
								D3DCOLOR_XRGB(255,255,255)); //draw the sprite

			DisplayGrid();

			DisplayCharacters();

			m_pD3DSprite->End();

			m_pD3DDevice->EndScene();
			m_pD3DDevice->Present(0,0,0,0);
		}
		if (levelState == LOAD5)
		{
			gameWin = false;
			for(int i = 0; i < 10; i++)
			{
				for(int j = 0; j < 10; j++)
					grid[i][j] = 0;
			}
			for(int j = 0; j < 2; j++)
			{
				for(int i = 0; i < 10; i++)
					grid[i][j] = -1;
			}
			for(int j = 9; j < 10; j++)
			{
				for(int i = 0; i < 10; i++)
					grid[i][j] = -1;
			}
			for(int i = 0; i < 2; i++)
			{
				for(int j = 0; j < 10; j++)
					grid[i][j] = -1;
			}
			for(int i = 9; i < 10; i++)
			{
				for(int j = 0; j < 10; j++)
					grid[i][j] = -1;
			}
			playerPos[X] = 6;
			playerPos[Y] = 6;
			grid[playerPos[X]-1][playerPos[Y]-1] = 1;
			playerDying = false;
			levelChanging = false;
			aI1Pos[X] = 5;
			aI1Pos[Y] = 5;
			grid[aI1Pos[X]-1][aI1Pos[Y]-1] = 2;
			aI2Pos[X] = 7;
			aI2Pos[Y] = 7;
			grid[aI2Pos[X]-1][aI2Pos[Y]-1] = 2;
			aI3Pos[X] = 8;
			aI3Pos[Y] = 5;
			grid[aI3Pos[X]-1][aI3Pos[Y]-1] = 2;
			aI4Pos[X] = 4;
			aI4Pos[Y] = 7;
			grid[aI4Pos[X]-1][aI4Pos[Y]-1] = 2;
			aI5Pos[X] = 6;
			aI5Pos[Y] = 9;
			grid[aI5Pos[X]-1][aI5Pos[Y]-1] = 2;
			aI6Pos[X] = 6;
			aI6Pos[Y] = 3;
			grid[aI6Pos[X]-1][aI6Pos[Y]-1] = 2;
			aI7Pos[X] = 3;
			aI7Pos[Y] = 4;
			grid[aI7Pos[X]-1][aI7Pos[Y]-1] = 2;
			aI8Pos[X] = 9;
			aI8Pos[Y] = 8;
			grid[aI8Pos[X]-1][aI8Pos[Y]-1] = 2;
			aI9Pos[X] = 4;
			aI9Pos[Y] = 4;
			grid[aI9Pos[X]-1][aI9Pos[Y]-1] = 2;
			aI10Pos[X] = 8;
			aI10Pos[Y] = 8;
			grid[aI10Pos[X]-1][aI10Pos[Y]-1] = 2;

			player = Player(Vector3D(grid_size * playerPos[X] - grid_size/2.0,
								 grid_size * playerPos[Y] - grid_size/2.0, 0),
								 Vector3D(800,600,0),grid_size,m_pD3DDevice);
			ai_1 = AIUnit(Vector3D(grid_size * aI1Pos[X] - grid_size/2.0,
								 grid_size * aI1Pos[Y] - grid_size/2.0, 0),
								 Vector3D(800,600,0),grid_size,m_pD3DDevice,1);
			ai_2 = AIUnit(Vector3D(grid_size * aI2Pos[X] - grid_size/2.0,
								 grid_size * aI2Pos[Y] - grid_size/2.0, 0),
								 Vector3D(800,600,0),grid_size,m_pD3DDevice,1);
			ai_3 = AIUnit(Vector3D(grid_size * aI3Pos[X] - grid_size/2.0,
								 grid_size * aI3Pos[Y] - grid_size/2.0, 0),
								 Vector3D(800,600,0),grid_size,m_pD3DDevice,1);
			ai_4 = AIUnit(Vector3D(grid_size * aI4Pos[X] - grid_size/2.0,
								 grid_size * aI4Pos[Y] - grid_size/2.0, 0),
								 Vector3D(800,600,0),grid_size,m_pD3DDevice,1);
			ai_5 = AIUnit(Vector3D(grid_size * aI5Pos[X] - grid_size/2.0,
								 grid_size * aI5Pos[Y] - grid_size/2.0, 0),
								 Vector3D(800,600,0),grid_size,m_pD3DDevice,2);
			ai_6 = AIUnit(Vector3D(grid_size * aI6Pos[X] - grid_size/2.0,
								 grid_size * aI6Pos[Y] - grid_size/2.0, 0),
								 Vector3D(800,600,0),grid_size,m_pD3DDevice,2);
			ai_7 = AIUnit(Vector3D(grid_size * aI7Pos[X] - grid_size/2.0,
								 grid_size * aI7Pos[Y] - grid_size/2.0, 0),
								 Vector3D(800,600,0),grid_size,m_pD3DDevice,2);
			ai_8 = AIUnit(Vector3D(grid_size * aI8Pos[X] - grid_size/2.0,
								 grid_size * aI8Pos[Y] - grid_size/2.0, 0),
								 Vector3D(800,600,0),grid_size,m_pD3DDevice,2);
			ai_9 = AIUnit(Vector3D(grid_size * aI9Pos[X] - grid_size/2.0,
								 grid_size * aI9Pos[Y] - grid_size/2.0, 0),
								 Vector3D(800,600,0),grid_size,m_pD3DDevice,1);
			ai_10 = AIUnit(Vector3D(grid_size * aI10Pos[X] - grid_size/2.0,
								 grid_size * aI10Pos[Y] - grid_size/2.0, 0),
								 Vector3D(800,600,0),grid_size,m_pD3DDevice,1);
			levelState = LEVELFIVE;
		}
		if (levelState == LEVELFIVE)
		{
			// Clear the back buffer, call BeginScene()
			m_pD3DDevice->Clear(0,0,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(255,255,255), 1.0f, 0);
			m_pD3DDevice->BeginScene();

			m_pD3DSprite->Begin(D3DXSPRITE_ALPHABLEND);

			D3DXVECTOR3 center(0,0,0); //set the center of the sprite
			m_pD3DSprite->SetTransform(&(menuBackgroundSetup.CreateWorldMat()));
			m_pD3DSprite->Draw(menuBackground,0,&center,0,
							   D3DCOLOR_XRGB(255,255,255)); //draw the sprite

			POINT mousePos;
			bool mouseOff = true;
		
			GetCursorPos(&mousePos);
			ScreenToClient(m_hWnd,&mousePos);

			if (	mousePos.x > (mainButtonTex_Set.transMat._41 - mainSelectTex_I.Width/2.0f)-20 &&
					mousePos.x < (mainButtonTex_Set.transMat._41 + mainSelectTex_I.Width/2.0f)-15 &&
					mousePos.y > (mainButtonTex_Set.transMat._42 - mainSelectTex_I.Height/2.0f)-64&&
					mousePos.y < (mainButtonTex_Set.transMat._42 + mainSelectTex_I.Height/2.0f)-64)
			{
				if (buttonSelect != PLAY)
				{
					int result = system->playSound(FMOD_CHANNEL_FREE, sound3, false, &myChannel2);
					ERRCHECK(result);
					result = myChannel2->setVolume(0.5);
					ERRCHECK(result); // displays message if this returned an error
				}
				buttonSelect = PLAY;
				mouseOff = false;
			}
			else if (	mousePos.x > (restartButtonTex_Set.transMat._41 - restartSelectTex_I.Width/2.0f)-20 &&
						mousePos.x < (restartButtonTex_Set.transMat._41 + restartSelectTex_I.Width/2.0f)-15 &&
						mousePos.y > (restartButtonTex_Set.transMat._42 - restartSelectTex_I.Height/2.0f)-68&&
						mousePos.y < (restartButtonTex_Set.transMat._42 + restartSelectTex_I.Height/2.0f)-68)
			{
				if (buttonSelect != CREDITS)
				{
					int result = system->playSound(FMOD_CHANNEL_FREE, sound3, false, &myChannel2);
					ERRCHECK(result);
					result = myChannel2->setVolume(0.5);
					ERRCHECK(result); // displays message if this returned an error
				}
				buttonSelect = CREDITS;
				mouseOff = false;
			}
			else
				buttonSelect = MAIN;

			if (!mouseOff && mouseState.rgbButtons[0] & 0x80 && keyboard_delay == 0)
			{
				if(buttonSelect == PLAY)
				{
					gameState = MENU;
					buttonSelect = MAIN;
					myChannel->stop();
					FMOD_RESULT result;
					result = system->playSound(FMOD_CHANNEL_FREE, myStream,
								   false, &myChannel);
					ERRCHECK(result);
					result = system->playSound(FMOD_CHANNEL_FREE, sound5, false, &myChannel2);
					ERRCHECK(result);
					result = myChannel2->setVolume(0.5);
					ERRCHECK(result); // displays message if this returned an error
					keyboard_delay = 10.0;
				}
				if(buttonSelect == CREDITS)
				{
					levelState = LOAD5;
					int result = system->playSound(FMOD_CHANNEL_FREE, sound5, false, &myChannel2);
					ERRCHECK(result);
					result = myChannel2->setVolume(0.5);
					ERRCHECK(result); // displays message if this returned an error
					keyboard_delay = 10.0;
				}
			}
			if (buffer[DIK_R] & 0x80 && keyboard_delay == 0)
			{
				levelState = LOAD5;
				int result = system->playSound(FMOD_CHANNEL_FREE, sound5, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				keyboard_delay = 10.0;
			}

			if (buttonSelect == PLAY)
			{
				center = D3DXVECTOR3(mainSelectTex_I.Width/2.0f,mainSelectTex_I.Width/2.0f,0); //set the center of the sprite
				m_pD3DSprite->SetTransform(&(mainSelectTex_Set.CreateWorldMat()));
				m_pD3DSprite->Draw(mainSelectTex,0,&center,0,
								   D3DCOLOR_XRGB(255,255,255)); //draw the sprite
			}
			else
			{
				center = D3DXVECTOR3(mainButtonTex_I.Width/2.0f,mainButtonTex_I.Width/2.0f,0); //set the center of the sprite
				m_pD3DSprite->SetTransform(&(mainButtonTex_Set.CreateWorldMat()));
				m_pD3DSprite->Draw(mainButtonTex,0,&center,0,
								   D3DCOLOR_XRGB(255,255,255)); //draw the sprite
			}

			if (buttonSelect == CREDITS)
			{
				center = D3DXVECTOR3(restartSelectTex_I.Width/2.0f,restartSelectTex_I.Width/2.0f,0); //set the center of the sprite
				m_pD3DSprite->SetTransform(&(restartSelectTex_Set.CreateWorldMat()));
				m_pD3DSprite->Draw(restartSelectTex,0,&center,0,
								   D3DCOLOR_XRGB(255,255,255)); //draw the sprite
			}
			else
			{
				center = D3DXVECTOR3(restartButtonTex_I.Width/2.0f,restartButtonTex_I.Width/2.0f,0); //set the center of the sprite
				m_pD3DSprite->SetTransform(&(restartButtonTex_Set.CreateWorldMat()));
				m_pD3DSprite->Draw(restartButtonTex,0,&center,0,
								   D3DCOLOR_XRGB(255,255,255)); //draw the sprite
			}

			center = D3DXVECTOR3(LevelFiveTex_I.Width/2.0f,LevelFiveTex_I.Width/2.0f,0); //set the center of the sprite
			m_pD3DSprite->SetTransform(&(LevelFiveTex_Set.CreateWorldMat()));
			m_pD3DSprite->Draw(LevelFiveTex,0,&center,0,
								D3DCOLOR_XRGB(255,255,255)); //draw the sprite

			DisplayGrid();

			DisplayCharacters();

			if (gameWin == true)
			{
				center = D3DXVECTOR3(WinTex_I.Width/2.0f,WinTex_I.Width/2.0f,0); //set the center of the sprite
				m_pD3DSprite->SetTransform(&(WinTex_Set.CreateWorldMat()));
				m_pD3DSprite->Draw(WinTex,0,&center,0,
								   D3DCOLOR_XRGB(255,255,255)); //draw the sprite
			}

			m_pD3DSprite->End();

			m_pD3DDevice->EndScene();
			m_pD3DDevice->Present(0,0,0,0);
		}
	}
}

void CDirectXFramework::Shutdown()
{
	//*************************************************************************
	// Release COM objects in the opposite order they were created in

	// Texture

	// Sprite

	// Font

	// 3DDevice	
		
	// 3DObject

	//*************************************************************************

	m_pDIKeyboard->Unacquire();

	m_pDIMouse->Unacquire();

	SAFE_RELEASE(m_pDIObject);

	SAFE_RELEASE(m_pTexture);

	SAFE_RELEASE(m_pD3DSprite);

	SAFE_RELEASE(m_pD3DFont);

	SAFE_RELEASE(m_pD3DDevice);

	SAFE_RELEASE(m_pD3DObject);
}

bool CDirectXFramework::Quit()
{
	return quit;
}

void CDirectXFramework::DisplayGrid()
{
	for(int i = 0; i <= maxGrid; i++)
	{
		for(int j = 0; j <= maxGrid; j++)
		{
			if (grid[i][j] != -1)
			{
				D3DXVECTOR3 center(0,0,0); //set the center of the sprite

				basicTileTex_Set.transMat._41 = i * grid_size;
				basicTileTex_Set.transMat._42 = j * grid_size;

				m_pD3DSprite->SetTransform(&(basicTileTex_Set.CreateWorldMat()));
				m_pD3DSprite->Draw(basicTileTex,0,&center,0,
									D3DCOLOR_XRGB(255,255,255)); //draw the sprite
			}
		}
	}
}

bool CDirectXFramework::CheckPCollision()
{
	if (collision == 1)
	{
		if (grid[playerPos[X]-1][playerPos[Y]-2] == 2)
		{
			if (aI1Pos[X]-1 == playerPos[X]-1 &&
				aI1Pos[Y]-1 == playerPos[Y]-2)
			{
				ai_1.Kill(grid,aI1Pos);
				int result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				return true;
			}
			if (aI2Pos[X]-1 == playerPos[X]-1 &&
				aI2Pos[Y]-1 == playerPos[Y]-2)
			{
				ai_2.Kill(grid,aI2Pos);
				int result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				return true;
			}
			if (aI3Pos[X]-1 == playerPos[X]-1 &&
				aI3Pos[Y]-1 == playerPos[Y]-2)
			{
				ai_3.Kill(grid,aI3Pos);
				int result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				return true;
			}
			if (aI4Pos[X]-1 == playerPos[X]-1 &&
				aI4Pos[Y]-1 == playerPos[Y]-2)
			{
				ai_4.Kill(grid,aI4Pos);
				int result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				return true;
			}
			if (aI5Pos[X]-1 == playerPos[X]-1 &&
				aI5Pos[Y]-1 == playerPos[Y]-2)
			{
				ai_5.Kill(grid,aI5Pos);
				int result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				return true;
			}
			if (aI6Pos[X]-1 == playerPos[X]-1 &&
				aI6Pos[Y]-1 == playerPos[Y]-2)
			{
				ai_6.Kill(grid,aI6Pos);
				int result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				return true;
			}
			if (aI7Pos[X]-1 == playerPos[X]-1 &&
				aI7Pos[Y]-1 == playerPos[Y]-2)
			{
				ai_7.Kill(grid,aI7Pos);
				int result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				return true;
			}
			if (aI8Pos[X]-1 == playerPos[X]-1 &&
				aI8Pos[Y]-1 == playerPos[Y]-2)
			{
				ai_8.Kill(grid,aI8Pos);
				int result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				return true;
			}
			if (aI9Pos[X]-1 == playerPos[X]-1 &&
				aI9Pos[Y]-1 == playerPos[Y]-2)
			{
				ai_9.Kill(grid,aI9Pos);
				int result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				return true;
			}
			if (aI10Pos[X]-1 == playerPos[X]-1 &&
				aI10Pos[Y]-1 == playerPos[Y]-2)
			{
				ai_10.Kill(grid,aI10Pos);
				int result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				return true;
			}
		}
	}
	if (collision == 2)
	{
		if (grid[playerPos[X]][playerPos[Y]-1] == 2)
		{
			if (aI1Pos[X]-1 == playerPos[X] &&
				aI1Pos[Y]-1 == playerPos[Y]-1)
			{
				ai_1.Kill(grid,aI1Pos);
				int result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				return true;
			}
			if (aI2Pos[X]-1 == playerPos[X] &&
				aI2Pos[Y]-1 == playerPos[Y]-1)
			{
				ai_2.Kill(grid,aI2Pos);
				int result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				return true;
			}
			if (aI3Pos[X]-1 == playerPos[X] &&
				aI3Pos[Y]-1 == playerPos[Y]-1)
			{
				ai_3.Kill(grid,aI3Pos);
				int result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				return true;
			}
			if (aI4Pos[X]-1 == playerPos[X] &&
				aI4Pos[Y]-1 == playerPos[Y]-1)
			{
				ai_4.Kill(grid,aI4Pos);
				int result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				return true;
			}
			if (aI5Pos[X]-1 == playerPos[X] &&
				aI5Pos[Y]-1 == playerPos[Y]-1)
			{
				ai_5.Kill(grid,aI5Pos);
				int result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				return true;
			}
			if (aI6Pos[X]-1 == playerPos[X] &&
				aI6Pos[Y]-1 == playerPos[Y]-1)
			{
				ai_6.Kill(grid,aI6Pos);
				int result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				return true;
			}
			if (aI7Pos[X]-1 == playerPos[X] &&
				aI7Pos[Y]-1 == playerPos[Y]-1)
			{
				ai_7.Kill(grid,aI7Pos);
				int result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				return true;
			}
			if (aI8Pos[X]-1 == playerPos[X] &&
				aI8Pos[Y]-1 == playerPos[Y]-1)
			{
				ai_8.Kill(grid,aI8Pos);
				int result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				return true;
			}
			if (aI9Pos[X]-1 == playerPos[X] &&
				aI9Pos[Y]-1 == playerPos[Y]-1)
			{
				ai_9.Kill(grid,aI9Pos);
				int result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				return true;
			}
			if (aI10Pos[X]-1 == playerPos[X] &&
				aI10Pos[Y]-1 == playerPos[Y]-1)
			{
				ai_10.Kill(grid,aI10Pos);
				int result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				return true;
			}
		}
	}
	if (collision == 3)
	{
		if (grid[playerPos[X]-1][playerPos[Y]] == 2)
		{
			if (aI1Pos[X]-1 == playerPos[X]-1 &&
				aI1Pos[Y]-1 == playerPos[Y])
			{
				ai_1.Kill(grid,aI1Pos);
				int result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				return true;
			}
			if (aI2Pos[X]-1 == playerPos[X]-1 &&
				aI2Pos[Y]-1 == playerPos[Y])
			{
				ai_2.Kill(grid,aI2Pos);
				int result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				return true;
			}
			if (aI3Pos[X]-1 == playerPos[X]-1 &&
				aI3Pos[Y]-1 == playerPos[Y])
			{
				ai_3.Kill(grid,aI3Pos);
				int result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				return true;
			}
			if (aI4Pos[X]-1 == playerPos[X]-1 &&
				aI4Pos[Y]-1 == playerPos[Y])
			{
				ai_4.Kill(grid,aI4Pos);
				int result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				return true;
			}
			if (aI5Pos[X]-1 == playerPos[X]-1 &&
				aI5Pos[Y]-1 == playerPos[Y])
			{
				ai_5.Kill(grid,aI5Pos);
				int result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				return true;
			}
			if (aI6Pos[X]-1 == playerPos[X]-1 &&
				aI6Pos[Y]-1 == playerPos[Y])
			{
				ai_6.Kill(grid,aI6Pos);
				int result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				return true;
			}
			if (aI7Pos[X]-1 == playerPos[X]-1 &&
				aI7Pos[Y]-1 == playerPos[Y])
			{
				ai_7.Kill(grid,aI7Pos);
				int result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				return true;
			}
			if (aI8Pos[X]-1 == playerPos[X]-1 &&
				aI8Pos[Y]-1 == playerPos[Y])
			{
				ai_8.Kill(grid,aI8Pos);
				int result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				return true;
			}
			if (aI9Pos[X]-1 == playerPos[X]-1 &&
				aI9Pos[Y]-1 == playerPos[Y])
			{
				ai_9.Kill(grid,aI9Pos);
				int result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				return true;
			}
			if (aI10Pos[X]-1 == playerPos[X]-1 &&
				aI10Pos[Y]-1 == playerPos[Y])
			{
				ai_10.Kill(grid,aI10Pos);
				int result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				return true;
			}
		}
	}
	if (collision == 4)
	{
		if (grid[playerPos[X]-2][playerPos[Y]-1] == 2)
		{
			if (aI1Pos[X]-1 == playerPos[X]-2 &&
				aI1Pos[Y]-1 == playerPos[Y]-1)
			{
				ai_1.Kill(grid,aI1Pos);
				int result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				return true;
			}
			if (aI2Pos[X]-1 == playerPos[X]-2 &&
				aI2Pos[Y]-1 == playerPos[Y]-1)
			{
				ai_2.Kill(grid,aI2Pos);
				int result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				return true;
			}
			if (aI3Pos[X]-1 == playerPos[X]-2 &&
				aI3Pos[Y]-1 == playerPos[Y]-1)
			{
				ai_3.Kill(grid,aI3Pos);
				int result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				return true;
			}
			if (aI4Pos[X]-1 == playerPos[X]-2 &&
				aI4Pos[Y]-1 == playerPos[Y]-1)
			{
				ai_4.Kill(grid,aI4Pos);
				int result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				return true;
			}
			if (aI5Pos[X]-1 == playerPos[X]-2 &&
				aI5Pos[Y]-1 == playerPos[Y]-1)
			{
				ai_5.Kill(grid,aI5Pos);
				int result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				return true;
			}
			if (aI6Pos[X]-1 == playerPos[X]-2 &&
				aI6Pos[Y]-1 == playerPos[Y]-1)
			{
				ai_6.Kill(grid,aI6Pos);
				int result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				return true;
			}
			if (aI7Pos[X]-1 == playerPos[X]-2 &&
				aI7Pos[Y]-1 == playerPos[Y]-1)
			{
				ai_7.Kill(grid,aI7Pos);
				int result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				return true;
			}
			if (aI8Pos[X]-1 == playerPos[X]-2 &&
				aI8Pos[Y]-1 == playerPos[Y]-1)
			{
				ai_8.Kill(grid,aI8Pos);
				int result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				return true;
			}
			if (aI9Pos[X]-1 == playerPos[X]-2 &&
				aI9Pos[Y]-1 == playerPos[Y]-1)
			{
				ai_9.Kill(grid,aI9Pos);
				int result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				return true;
			}
			if (aI10Pos[X]-1 == playerPos[X]-2 &&
				aI10Pos[Y]-1 == playerPos[Y]-1)
			{
				ai_10.Kill(grid,aI10Pos);
				int result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &myChannel2);
				ERRCHECK(result);
				result = myChannel2->setVolume(0.5);
				ERRCHECK(result); // displays message if this returned an error
				return true;
			}
		}
	}
	return false;
}

void CDirectXFramework::DisplayCharacters()
{
	D3DXVECTOR3 center;
	if (player.exists)
	{
		center = D3DXVECTOR3(player.characterTex_I.Width/2.0f,player.characterTex_I.Width/2.0f,0); //set the center of the sprite
		m_pD3DSprite->SetTransform(&(player.characterTex_Set.CreateWorldMat()));
		m_pD3DSprite->Draw(player.characterTex,0,&center,0,
							D3DCOLOR_XRGB(255,255,255)); //draw the sprite
	}

	if (!ai_1.dead)
	{
		center = D3DXVECTOR3(ai_1.characterTex_I.Width/2.0f,ai_1.characterTex_I.Width/2.0f,0); //set the center of the sprite
		m_pD3DSprite->SetTransform(&(ai_1.characterTex_Set.CreateWorldMat()));
		m_pD3DSprite->Draw(ai_1.characterTex,0,&center,0,
							D3DCOLOR_XRGB(255,255,255)); //draw the sprite
	}
	if (!ai_2.dead)
	{
		center = D3DXVECTOR3(ai_2.characterTex_I.Width/2.0f,ai_2.characterTex_I.Width/2.0f,0); //set the center of the sprite
		m_pD3DSprite->SetTransform(&(ai_2.characterTex_Set.CreateWorldMat()));
		m_pD3DSprite->Draw(ai_2.characterTex,0,&center,0,
							D3DCOLOR_XRGB(255,255,255)); //draw the sprite
	}
	if (!ai_3.dead)
	{
		center = D3DXVECTOR3(ai_3.characterTex_I.Width/2.0f,ai_3.characterTex_I.Width/2.0f,0); //set the center of the sprite
		m_pD3DSprite->SetTransform(&(ai_3.characterTex_Set.CreateWorldMat()));
		m_pD3DSprite->Draw(ai_3.characterTex,0,&center,0,
							D3DCOLOR_XRGB(255,255,255)); //draw the sprite
	}
	if (!ai_4.dead)
	{
		center = D3DXVECTOR3(ai_4.characterTex_I.Width/2.0f,ai_4.characterTex_I.Width/2.0f,0); //set the center of the sprite
		m_pD3DSprite->SetTransform(&(ai_4.characterTex_Set.CreateWorldMat()));
		m_pD3DSprite->Draw(ai_4.characterTex,0,&center,0,
							D3DCOLOR_XRGB(255,255,255)); //draw the sprite
	}
	if (!ai_5.dead)
	{
		center = D3DXVECTOR3(ai_5.characterTex_I.Width/2.0f,ai_5.characterTex_I.Width/2.0f,0); //set the center of the sprite
		m_pD3DSprite->SetTransform(&(ai_5.characterTex_Set.CreateWorldMat()));
		m_pD3DSprite->Draw(ai_5.characterTex,0,&center,0,
							D3DCOLOR_XRGB(255,255,255)); //draw the sprite
	}
	if (!ai_6.dead)
	{
		center = D3DXVECTOR3(ai_6.characterTex_I.Width/2.0f,ai_6.characterTex_I.Width/2.0f,0); //set the center of the sprite
		m_pD3DSprite->SetTransform(&(ai_6.characterTex_Set.CreateWorldMat()));
		m_pD3DSprite->Draw(ai_6.characterTex,0,&center,0,
							D3DCOLOR_XRGB(255,255,255)); //draw the sprite
	}
	if (!ai_7.dead)
	{
		center = D3DXVECTOR3(ai_7.characterTex_I.Width/2.0f,ai_7.characterTex_I.Width/2.0f,0); //set the center of the sprite
		m_pD3DSprite->SetTransform(&(ai_7.characterTex_Set.CreateWorldMat()));
		m_pD3DSprite->Draw(ai_7.characterTex,0,&center,0,
							D3DCOLOR_XRGB(255,255,255)); //draw the sprite
	}
	if (!ai_8.dead)
	{
		center = D3DXVECTOR3(ai_8.characterTex_I.Width/2.0f,ai_8.characterTex_I.Width/2.0f,0); //set the center of the sprite
		m_pD3DSprite->SetTransform(&(ai_8.characterTex_Set.CreateWorldMat()));
		m_pD3DSprite->Draw(ai_8.characterTex,0,&center,0,
							D3DCOLOR_XRGB(255,255,255)); //draw the sprite
	}
	if (!ai_9.dead)
	{
		center = D3DXVECTOR3(ai_9.characterTex_I.Width/2.0f,ai_9.characterTex_I.Width/2.0f,0); //set the center of the sprite
		m_pD3DSprite->SetTransform(&(ai_9.characterTex_Set.CreateWorldMat()));
		m_pD3DSprite->Draw(ai_9.characterTex,0,&center,0,
							D3DCOLOR_XRGB(255,255,255)); //draw the sprite
	}
	if (!ai_10.dead)
	{
		center = D3DXVECTOR3(ai_10.characterTex_I.Width/2.0f,ai_10.characterTex_I.Width/2.0f,0); //set the center of the sprite
		m_pD3DSprite->SetTransform(&(ai_10.characterTex_Set.CreateWorldMat()));
		m_pD3DSprite->Draw(ai_10.characterTex,0,&center,0,
							D3DCOLOR_XRGB(255,255,255)); //draw the sprite
	}
}

bool CDirectXFramework::AllEnemiesDead()
{
	bool allDead = true;
	allDead &= ai_1.dead;
	allDead &= ai_2.dead;
	allDead &= ai_3.dead;
	allDead &= ai_4.dead;
	allDead &= ai_5.dead;
	allDead &= ai_6.dead;
	allDead &= ai_7.dead;
	allDead &= ai_8.dead;
	allDead &= ai_9.dead;
	allDead &= ai_10.dead;
	return allDead;
}