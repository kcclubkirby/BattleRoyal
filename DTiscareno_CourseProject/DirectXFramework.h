//////////////////////////////////////////////////////////////////////////
// Name:	DirectXFramework.h
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
#pragma once
#pragma comment(lib, "winmm.lib")
//////////////////////////////////////////////////////////////////////////
// Direct3D 9 headers and libraries required
//////////////////////////////////////////////////////////////////////////
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <time.h>
#include <dshow.h>
#include <fmod.hpp>
#include "CharacterUnit.h"
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "Strmiids.lib")
#pragma comment(lib, "fmodex_vc.lib")

// Macro to release COM objects fast and safely
#define SAFE_RELEASE(x) if(x){x->Release(); x = 0;}
#define ERRCHECK(x) if(x != FMOD_OK) {MessageBox(m_hWnd, (LPCWSTR)x, L"Fmod Error", MB_OK);}
static const Scalar grid_size = 50.0f;

class CDirectXFramework
{
	//////////////////////////////////////////////////////////////////////////
	// Application Variables
	//////////////////////////////////////////////////////////////////////////
	HWND				m_hWnd;			// Handle to the window
	bool				m_bVsync;		// Boolean for vertical syncing

	//////////////////////////////////////////////////////////////////////////
	// Direct3D Variables
	//////////////////////////////////////////////////////////////////////////
	IDirect3D9*			m_pD3DObject;	// Direct3D 9 Object
	IDirect3DDevice9*	m_pD3DDevice;	// Direct3D 9 Device
	D3DCAPS9			m_D3DCaps;		// Device Capabilities

	//////////////////////////////////////////////////////////////////////////
	// Font Variables
	//////////////////////////////////////////////////////////////////////////
	ID3DXFont*			m_pD3DFont;		// Font Object

	//////////////////////////////////////////////////////////////////////////
	// Sprite Variables
	//////////////////////////////////////////////////////////////////////////
	ID3DXSprite*		m_pD3DSprite;	// Sprite Object
	IDirect3DTexture9*	m_pTexture;		// Texture Object for a sprite
	D3DXIMAGE_INFO		m_imageInfo;	// File details of a texture

	spriteSetup sprites[6]; //the different sprite informations

	//The fps variables
	float fps;
	float lastTickCount;
	float currentTickCount;
	char cFrameRate[50];

	IDirectInput8* m_pDIObject;
	IDirectInputDevice8* m_pDIKeyboard;
	IDirectInputDevice8* m_pDIMouse;
	IGraphBuilder* m_pGraphBuilder;
	IMediaControl* m_pMediaControl;
	IMediaEvent* m_pMediaEvent;
	IVideoWindow* m_pVideoWindow;

	char buffer[256];
	DIMOUSESTATE2 mouseState;

	IDirect3DTexture9*	menuBackground;
	D3DXIMAGE_INFO		menuBackInfo;
	spriteSetup menuBackgroundSetup;

	IDirect3DTexture9*	titleTex;
	D3DXIMAGE_INFO		titleTex_I;
	spriteSetup titleTex_Set;

	IDirect3DTexture9*	playButtonTex;
	D3DXIMAGE_INFO		playButtonTex_I;
	spriteSetup playButtonTex_Set;

	IDirect3DTexture9*	playSelectTex;
	D3DXIMAGE_INFO		playSelectTex_I;
	spriteSetup playSelectTex_Set;

	IDirect3DTexture9*	credButtonTex;
	D3DXIMAGE_INFO		credButtonTex_I;
	spriteSetup credButtonTex_Set;

	IDirect3DTexture9*	credSelectTex;
	D3DXIMAGE_INFO		credSelectTex_I;
	spriteSetup credSelectTex_Set;

	IDirect3DTexture9*	optButtonTex;
	D3DXIMAGE_INFO		optButtonTex_I;
	spriteSetup optButtonTex_Set;

	IDirect3DTexture9*	optSelectTex;
	D3DXIMAGE_INFO		optSelectTex_I;
	spriteSetup optSelectTex_Set;

	IDirect3DTexture9*	quitButtonTex;
	D3DXIMAGE_INFO		quitButtonTex_I;
	spriteSetup quitButtonTex_Set;

	IDirect3DTexture9*	quitSelectTex;
	D3DXIMAGE_INFO		quitSelectTex_I;
	spriteSetup quitSelectTex_Set;

	IDirect3DTexture9*	backButtonTex;
	D3DXIMAGE_INFO		backButtonTex_I;
	spriteSetup backButtonTex_Set;

	IDirect3DTexture9*	backSelectTex;
	D3DXIMAGE_INFO		backSelectTex_I;
	spriteSetup backSelectTex_Set;

	IDirect3DTexture9*	fScreenButtonTex;
	D3DXIMAGE_INFO		fScreenButtonTex_I;
	spriteSetup fScreenButtonTex_Set;

	IDirect3DTexture9*	fScreenSelectTex;
	D3DXIMAGE_INFO		fScreenSelectTex_I;
	spriteSetup fScreenSelectTex_Set;

	IDirect3DTexture9*	windButtonTex;
	D3DXIMAGE_INFO		windButtonTex_I;
	spriteSetup windButtonTex_Set;

	IDirect3DTexture9*	windSelectTex;
	D3DXIMAGE_INFO		windSelectTex_I;
	spriteSetup windSelectTex_Set;

	IDirect3DTexture9*	creditsTex;
	D3DXIMAGE_INFO		creditsTex_I;
	spriteSetup creditsTex_Set;
	
	IDirect3DTexture9*	basicTileTex;
	D3DXIMAGE_INFO		basicTileTex_I;
	spriteSetup basicTileTex_Set;

	IDirect3DTexture9*	mainButtonTex;
	D3DXIMAGE_INFO		mainButtonTex_I;
	spriteSetup mainButtonTex_Set;

	IDirect3DTexture9*	mainSelectTex;
	D3DXIMAGE_INFO		mainSelectTex_I;
	spriteSetup mainSelectTex_Set;

	IDirect3DTexture9*	restartButtonTex;
	D3DXIMAGE_INFO		restartButtonTex_I;
	spriteSetup restartButtonTex_Set;

	IDirect3DTexture9*	restartSelectTex;
	D3DXIMAGE_INFO		restartSelectTex_I;
	spriteSetup restartSelectTex_Set;

	IDirect3DTexture9*	LevelOneTex;
	D3DXIMAGE_INFO		LevelOneTex_I;
	spriteSetup LevelOneTex_Set;

	IDirect3DTexture9*	LevelTwoTex;
	D3DXIMAGE_INFO		LevelTwoTex_I;
	spriteSetup LevelTwoTex_Set;

	IDirect3DTexture9*	LevelThreeTex;
	D3DXIMAGE_INFO		LevelThreeTex_I;
	spriteSetup LevelThreeTex_Set;

	IDirect3DTexture9*	LevelFourTex;
	D3DXIMAGE_INFO		LevelFourTex_I;
	spriteSetup LevelFourTex_Set;

	IDirect3DTexture9*	LevelFiveTex;
	D3DXIMAGE_INFO		LevelFiveTex_I;
	spriteSetup LevelFiveTex_Set;

	IDirect3DTexture9*	WinTex;
	D3DXIMAGE_INFO		WinTex_I;
	spriteSetup WinTex_Set;

	int gameState, menuState, buttonSelect, levelState;
	float keyboard_delay;
	enum {LOAD,INTRO,MENU,GAME,QUIT};
	enum {MAIN,PLAY,CREDITS,OPTIONS,QUITBUTTON};
	enum {LOAD1,LEVELONE,LOAD2,LEVELTWO,LOAD3,LEVELTHREE,
		  LOAD4,LEVELFOUR,LOAD5,LEVELFIVE};

	FMOD::Sound* sound1;
	FMOD::Sound* sound2;
	FMOD::Sound* sound3;
	FMOD::Sound* sound4;
	FMOD::Sound* sound5;
	FMOD::Sound* sound6;
	FMOD::Sound* myStream;
	FMOD::Sound* gameStream;
	FMOD::System* system;
	FMOD::Channel* myChannel;
	FMOD::Channel* myChannel2;

	bool quit;
	bool windowed;

	D3DPRESENT_PARAMETERS D3Dpp;
	HINSTANCE m_hInst;

	//Game objects
	Player player;
	AIUnit ai_1, ai_2, ai_3, ai_4, ai_5,
		   ai_6, ai_7, ai_8, ai_9, ai_10;
	int grid[10][10];
	int playerPos[2];
	int maxGrid;
	int aI1Pos[2], aI2Pos[2], aI3Pos[2], aI4Pos[2], aI5Pos[2],
		aI6Pos[2], aI7Pos[2], aI8Pos[2], aI9Pos[2], aI10Pos[2];
	int counter, deadCounter;
	bool moving,playerDying, levelChanging, gameWin;
	int collision;
	float turnCounter, pulseCount;
	int levelNum;

public:
	//////////////////////////////////////////////////////////////////////////
	// Init and Shutdown are preferred to constructors and destructor,
	// due to having more control when to explicitly call them when global.
	//////////////////////////////////////////////////////////////////////////
	CDirectXFramework(void);
	~CDirectXFramework(void);

	//////////////////////////////////////////////////////////////////////////
	// Name:		Init
	// Parameters:	HWND hWnd - Handle to the window for the application
	//				HINSTANCE hInst - Handle to the application instance
	//				bool bWindowed - Boolean to control windowed or full-screen
	// Return:		void
	// Description:	Ran once at the start.  Initialize DirectX components and 
	//				variables to control the application.  
	//////////////////////////////////////////////////////////////////////////
	void Init(HWND& hWnd, HINSTANCE& hInst, bool bWindowed);

	//////////////////////////////////////////////////////////////////////////
	// Name:		Update
	// Parameters:	float elapsedTime - Time that has elapsed since the last
	//					update call.
	// Return:		void
	// Description: Runs every frame, use dt to limit functionality called to
	//				a certain amount of elapsed time that has passed.  Used 
	//				for updating variables and processing input commands prior
	//				to calling render.
	//////////////////////////////////////////////////////////////////////////
	void Update();

	//////////////////////////////////////////////////////////////////////////
	// Name:		Render
	// Parameters:	float elapsedTime - Time that has elapsed since the last
	//					render call.
	// Return:		void
	// Description: Runs every frame, use dt to limit functionality called to
	//				a certain amount of elapsed time that has passed.  Render
	//				calls all draw call to render objects to the screen.
	//////////////////////////////////////////////////////////////////////////
	void Render();

	//////////////////////////////////////////////////////////////////////////
	// Name:		Shutdown
	// Parameters:	void
	// Return:		void
	// Description:	Runs once at the end of an application.  Destroy COM 
	//				objects and deallocate dynamic memory.
	//////////////////////////////////////////////////////////////////////////
	void Shutdown();

	//////////////////////////////////////////////////////////////////////////
	// Name:		Quit
	// Parameters:	void
	// Return:		bool
	// Description:	Checks to see if the application should close
	//////////////////////////////////////////////////////////////////////////
	bool Quit();

	//////////////////////////////////////////////////////////////////////////
	// Name:		DisplayGrid
	// Parameters:	void
	// Return:		void
	// Description:	Draws the entire grid
	//////////////////////////////////////////////////////////////////////////
	void DisplayGrid();

	//////////////////////////////////////////////////////////////////////////
	// Name:		CheckPCollision
	// Parameters:	void
	// Return:		bool
	// Description:	Checks to see if the collision with the player will
	//				destroy an object
	//////////////////////////////////////////////////////////////////////////
	bool CheckPCollision();

	//////////////////////////////////////////////////////////////////////////
	// Name:		DisplayCharacters
	// Parameters:	void
	// Return:		void
	// Description:	Draws all objects
	//////////////////////////////////////////////////////////////////////////
	void DisplayCharacters();

	//////////////////////////////////////////////////////////////////////////
	// Name:		AllEnemiesDead
	// Parameters:	void
	// Return:		bool
	// Description:	Checks to see if all enemies are dead
	//////////////////////////////////////////////////////////////////////////
	bool AllEnemiesDead();
};
