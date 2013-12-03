/*------------------------------------------------------*
*MODIFIED BY:											*
*Name: David Tiscareno									*
*Class: GSP361											*
*Course Project											*
*Date: 5/12/13											*
*CourseProject.cpp										*
*Defines the entry point for the windows application.	*
*I only added the different characters and changed the	*
*application title.										*
*-------------------------------------------------------*/

#include "pch.h"
#include <iostream>
#include <fstream>
#include "MathLib.h"
#include "GraphicsLib.h"
#include "GeometryLib.h"
#include "GameFramework.h"
#include "CharacterUnit.h"

static const int pixels_per_meter = 100 ;
static const int num_systems = 10;
static const int num_particles = 12;
static const int life_time = 100;
static const int window_size = 240;
static const Scalar grid_size = 24.0f;
static const Scalar spread = .2f;

class MathGame : public GameFramework
{
private:
	Player player;
	AIUnit ai;
	float timer, endTimer, pillTimer, dTimer, sTimer, diamTimer, spawnCount;
	int grid[10][10];
	int playerPos[2];
	int maxGrid;
	int aIPos[2];
	int counter;
	bool moving;
public:
	MathGame()
	{
		for(int i = 0; i < 10; i++)
		{
			for(int j = 0; j < 10; j++)
				grid[i][j] = 0;
		}
		grid[0][0] = 1;
		playerPos[X] = 1;
		playerPos[Y] = 1;
		grid[4][4] = 2;
		aIPos[X] = 5;
		aIPos[Y] = 5;

		maxGrid = 9;
		moving = false;

		player = Player(Vector3D(grid_size * playerPos[X] - grid_size/2.0,
								 grid_size * playerPos[Y] - grid_size/2.0, 0),
								 window_size, grid_size);
		ai = AIUnit(Vector3D(grid_size * aIPos[X] - grid_size/2.0,
								 grid_size * aIPos[Y] - grid_size/2.0, 0),
								 window_size, grid_size);
		srand (time(NULL));
	}

	void Update_Model()
	{
		if (!moving)
		{
			player.checkIfMove(maxGrid, playerPos);
			moving = player.Move(grid,playerPos);
			if (moving)
				ai.Move(grid,aIPos,maxGrid);
		}
		else
		{
			bool stillMoving = false;
			stillMoving |= player.Update(elapsed_time,playerPos);
			stillMoving |= ai.Update(elapsed_time,aIPos);
			moving = stillMoving;
		}
	}

	void Render_Model()
	{
		player.Render();
		ai.Render();
	}
} ;

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd )
{
	MathGame game ;

	game.Open_Window( hInstance, "GSP 361 AI Course Project", 0, 0, window_size, window_size, 0, 0, 0 ) ;
	game.Run() ;
}