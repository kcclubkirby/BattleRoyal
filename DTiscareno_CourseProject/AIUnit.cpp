/*------------------------------------------------------*
*CREATED BY:											*
*Name: David Tiscareno									*
*Class: GSP361											*
*Course Project											*
*Date: 6/26/13											*
*AIUnit.cpp												*
*This is the implementation of the AIUnit class.		*
*-------------------------------------------------------*/

#include "CharacterUnit.h"

AIUnit::AIUnit(Vector3D pos, Vector3D winSize, int grSize, LPDIRECT3DDEVICE9 gameDevice, int enType)	
	//This constructor will initialize the values where
	//pos is a Vector for the position,
{	
	kinematic = Kinematic(pos,0,Vector3D::Zero_Vector(),0); //Initializes the physics for the character

	if (enType == 1)
		D3DXCreateTextureFromFileEx(gameDevice,L"enemyDown.png",50,50,0,0,D3DFMT_UNKNOWN,
									D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
									D3DCOLOR_XRGB(255,0,255),&characterTex_I,0,
									&characterTex);
	if (enType == 2)
		D3DXCreateTextureFromFileEx(gameDevice,L"enemy2Down.png",50,50,0,0,D3DFMT_UNKNOWN,
									D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
									D3DCOLOR_XRGB(255,0,255),&characterTex_I,0,
									&characterTex);
	characterTex_Set.Initialize();
	characterTex_Set.transMat._41 = pos[X];
	characterTex_Set.transMat._42 = pos[Y];

	steering = SteeringOutput(Vector3D(0,0,0),0.0); //Initializes the steering at zero

	steeringNum = 3; //Starts the steering with Blended Steering
	direction = 0;

	//Initialize the booleans
	defeatPlayer = false;
	dead = false;

	windowSize = winSize; //set the window size
	gridSize = grSize; //the size of each grid space

	type = enType; //set the enemy type
	count = 0; //set the counter to zero
	if (type == 1)
		count = 1;
	look = 3;
}

bool AIUnit::Move(int grid[10][10], int aIPos[2], int maxGrid, int playerPos[2], LPDIRECT3DDEVICE9 gameDevice)
{
	if(dead)
		return false;
	if (type == 1)
	{
		if (count == 1)
		{
			count--;
			return false;
		}
		if (count == 0)
			count = 1;
	}
	if (abs(playerPos[Y] - aIPos[Y]) > abs(playerPos[X] - aIPos[X]))
	{
		if ((playerPos[Y] - aIPos[Y]) > 0)
		{
			if (grid[aIPos[X]-1][aIPos[Y]] == 0)
			{
				direction = 3;
				look = 3;
				if (type == 1)
					D3DXCreateTextureFromFileEx(gameDevice,L"enemyDown.png",50,50,0,0,D3DFMT_UNKNOWN,
												D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
												D3DCOLOR_XRGB(255,0,255),&characterTex_I,0,
												&characterTex);
				if (type == 2)
					D3DXCreateTextureFromFileEx(gameDevice,L"enemy2Down.png",50,50,0,0,D3DFMT_UNKNOWN,
												D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
												D3DCOLOR_XRGB(255,0,255),&characterTex_I,0,
												&characterTex);
			}
			else
			{
				direction = 0;
				if (grid[aIPos[X]-1][aIPos[Y]] == 1)
				{
					look = 3;
					if (type == 1)
						D3DXCreateTextureFromFileEx(gameDevice,L"enemyDown.png",50,50,0,0,D3DFMT_UNKNOWN,
													D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
													D3DCOLOR_XRGB(255,0,255),&characterTex_I,0,
													&characterTex);
					if (type == 2)
						D3DXCreateTextureFromFileEx(gameDevice,L"enemy2Down.png",50,50,0,0,D3DFMT_UNKNOWN,
													D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
													D3DCOLOR_XRGB(255,0,255),&characterTex_I,0,
													&characterTex);
					return true;
				}
			}
		}
		else
		{
			if (grid[aIPos[X]-1][aIPos[Y]-2] == 0)
			{
				direction = 1;
				look = 1;
				if (type == 1)
					D3DXCreateTextureFromFileEx(gameDevice,L"enemyUp.png",50,50,0,0,D3DFMT_UNKNOWN,
												D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
												D3DCOLOR_XRGB(255,0,255),&characterTex_I,0,
												&characterTex);
				if (type == 2)
					D3DXCreateTextureFromFileEx(gameDevice,L"enemy2Up.png",50,50,0,0,D3DFMT_UNKNOWN,
												D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
												D3DCOLOR_XRGB(255,0,255),&characterTex_I,0,
												&characterTex);
			}
			else
			{
				direction = 0;
				if (grid[aIPos[X]-1][aIPos[Y]-2] == 1)
				{
					look = 1;
					if (type == 1)
						D3DXCreateTextureFromFileEx(gameDevice,L"enemyUp.png",50,50,0,0,D3DFMT_UNKNOWN,
													D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
													D3DCOLOR_XRGB(255,0,255),&characterTex_I,0,
													&characterTex);
					if (type == 2)
						D3DXCreateTextureFromFileEx(gameDevice,L"enemy2Up.png",50,50,0,0,D3DFMT_UNKNOWN,
													D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
													D3DCOLOR_XRGB(255,0,255),&characterTex_I,0,
													&characterTex);
					return true;
				}
			}
		}
		if (direction == 0)
		{
			if ((playerPos[X] - aIPos[X]) > 0)
			{
				if (grid[aIPos[X]][aIPos[Y]-1] == 0)
				{
					direction = 2;
					look = 2;
					if (type == 1)
						D3DXCreateTextureFromFileEx(gameDevice,L"enemyRight.png",50,50,0,0,D3DFMT_UNKNOWN,
													D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
													D3DCOLOR_XRGB(255,0,255),&characterTex_I,0,
													&characterTex);
					if (type == 2)
						D3DXCreateTextureFromFileEx(gameDevice,L"enemy2Right.png",50,50,0,0,D3DFMT_UNKNOWN,
													D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
													D3DCOLOR_XRGB(255,0,255),&characterTex_I,0,
													&characterTex);
				}
				else
				{
					direction = 0;
					if (grid[aIPos[X]][aIPos[Y]-1] == 1)
					{
						look = 2;
						if (type == 1)
							D3DXCreateTextureFromFileEx(gameDevice,L"enemyRight.png",50,50,0,0,D3DFMT_UNKNOWN,
														D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
														D3DCOLOR_XRGB(255,0,255),&characterTex_I,0,
														&characterTex);
						if (type == 2)
							D3DXCreateTextureFromFileEx(gameDevice,L"enemy2Right.png",50,50,0,0,D3DFMT_UNKNOWN,
														D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
														D3DCOLOR_XRGB(255,0,255),&characterTex_I,0,
														&characterTex);
						return true;
					}
				}
			}
			else
			{
				if (grid[aIPos[X]-2][aIPos[Y]-1] == 0)
				{
					direction = 4;
					look = 4;
					if (type == 1)
						D3DXCreateTextureFromFileEx(gameDevice,L"enemyLeft.png",50,50,0,0,D3DFMT_UNKNOWN,
													D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
													D3DCOLOR_XRGB(255,0,255),&characterTex_I,0,
													&characterTex);
					if (type == 2)
						D3DXCreateTextureFromFileEx(gameDevice,L"enemy2Left.png",50,50,0,0,D3DFMT_UNKNOWN,
													D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
													D3DCOLOR_XRGB(255,0,255),&characterTex_I,0,
													&characterTex);
				}
				else
				{
					direction = 0;
					if (grid[aIPos[X]-2][aIPos[Y]-1] == 1)
					{
						look = 4;
						if (type == 1)
							D3DXCreateTextureFromFileEx(gameDevice,L"enemyLeft.png",50,50,0,0,D3DFMT_UNKNOWN,
														D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
														D3DCOLOR_XRGB(255,0,255),&characterTex_I,0,
														&characterTex);
						if (type == 2)
							D3DXCreateTextureFromFileEx(gameDevice,L"enemy2Left.png",50,50,0,0,D3DFMT_UNKNOWN,
														D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
														D3DCOLOR_XRGB(255,0,255),&characterTex_I,0,
														&characterTex);
						return true;
					}
				}
			}
		}
	}
	else
	{
		if ((playerPos[X] - aIPos[X]) > 0)
		{
			if (grid[aIPos[X]][aIPos[Y]-1] == 0)
			{
				direction = 2;
				look = 2;
				if (type == 1)
					D3DXCreateTextureFromFileEx(gameDevice,L"enemyRight.png",50,50,0,0,D3DFMT_UNKNOWN,
												D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
												D3DCOLOR_XRGB(255,0,255),&characterTex_I,0,
												&characterTex);
				if (type == 2)
					D3DXCreateTextureFromFileEx(gameDevice,L"enemy2Right.png",50,50,0,0,D3DFMT_UNKNOWN,
												D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
												D3DCOLOR_XRGB(255,0,255),&characterTex_I,0,
												&characterTex);
			}
			else
			{
				direction = 0;
				if (grid[aIPos[X]][aIPos[Y]-1] == 1)
				{
					look = 2;
					if (type == 1)
						D3DXCreateTextureFromFileEx(gameDevice,L"enemyRight.png",50,50,0,0,D3DFMT_UNKNOWN,
													D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
													D3DCOLOR_XRGB(255,0,255),&characterTex_I,0,
													&characterTex);
					if (type == 2)
						D3DXCreateTextureFromFileEx(gameDevice,L"enemy2Right.png",50,50,0,0,D3DFMT_UNKNOWN,
													D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
													D3DCOLOR_XRGB(255,0,255),&characterTex_I,0,
													&characterTex);
					return true;
				}
			}
		}
		else
		{
			if (grid[aIPos[X]-2][aIPos[Y]-1] == 0)
			{
				direction = 4;
				look = 4;
				if (type == 1)
					D3DXCreateTextureFromFileEx(gameDevice,L"enemyLeft.png",50,50,0,0,D3DFMT_UNKNOWN,
												D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
												D3DCOLOR_XRGB(255,0,255),&characterTex_I,0,
												&characterTex);
				if (type == 2)
					D3DXCreateTextureFromFileEx(gameDevice,L"enemy2Left.png",50,50,0,0,D3DFMT_UNKNOWN,
												D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
												D3DCOLOR_XRGB(255,0,255),&characterTex_I,0,
												&characterTex);
			}
			else
			{
				direction = 0;
				if (grid[aIPos[X]-2][aIPos[Y]-1] == 1)
				{
					look = 4;
					if (type == 1)
						D3DXCreateTextureFromFileEx(gameDevice,L"enemyLeft.png",50,50,0,0,D3DFMT_UNKNOWN,
													D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
													D3DCOLOR_XRGB(255,0,255),&characterTex_I,0,
													&characterTex);
					if (type == 2)
						D3DXCreateTextureFromFileEx(gameDevice,L"enemy2Left.png",50,50,0,0,D3DFMT_UNKNOWN,
													D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
													D3DCOLOR_XRGB(255,0,255),&characterTex_I,0,
													&characterTex);
					return true;
				}
			}
		}
		if (direction == 0)
		{
			if ((playerPos[Y] - aIPos[Y]) > 0)
			{
				if (grid[aIPos[X]-1][aIPos[Y]] == 0)
				{
					direction = 3;
					look = 3;
					if (type == 1)
						D3DXCreateTextureFromFileEx(gameDevice,L"enemyDown.png",50,50,0,0,D3DFMT_UNKNOWN,
													D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
													D3DCOLOR_XRGB(255,0,255),&characterTex_I,0,
													&characterTex);
					if (type == 2)
						D3DXCreateTextureFromFileEx(gameDevice,L"enemy2Down.png",50,50,0,0,D3DFMT_UNKNOWN,
													D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
													D3DCOLOR_XRGB(255,0,255),&characterTex_I,0,
													&characterTex);
				}
				else
				{
					direction = 0;
					if (grid[aIPos[X]-1][aIPos[Y]] == 1)
					{
						look = 3;
						if (type == 1)
							D3DXCreateTextureFromFileEx(gameDevice,L"enemyDown.png",50,50,0,0,D3DFMT_UNKNOWN,
														D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
														D3DCOLOR_XRGB(255,0,255),&characterTex_I,0,
														&characterTex);
						if (type == 2)
							D3DXCreateTextureFromFileEx(gameDevice,L"enemy2Down.png",50,50,0,0,D3DFMT_UNKNOWN,
														D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
														D3DCOLOR_XRGB(255,0,255),&characterTex_I,0,
														&characterTex);
						return true;
					}
				}
			}
			else
			{
				if (grid[aIPos[X]-1][aIPos[Y]-2] == 0)
				{
					direction = 1;
					look = 1;
					if (type == 1)
						D3DXCreateTextureFromFileEx(gameDevice,L"enemyUp.png",50,50,0,0,D3DFMT_UNKNOWN,
													D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
													D3DCOLOR_XRGB(255,0,255),&characterTex_I,0,
													&characterTex);
					if (type == 2)
						D3DXCreateTextureFromFileEx(gameDevice,L"enemy2Up.png",50,50,0,0,D3DFMT_UNKNOWN,
													D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
													D3DCOLOR_XRGB(255,0,255),&characterTex_I,0,
													&characterTex);
				}
				else
				{
					direction = 0;
					if (grid[aIPos[X]-1][aIPos[Y]-2] == 1)
					{
						look = 1;
						if (type == 1)
							D3DXCreateTextureFromFileEx(gameDevice,L"enemyUp.png",50,50,0,0,D3DFMT_UNKNOWN,
														D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
														D3DCOLOR_XRGB(255,0,255),&characterTex_I,0,
														&characterTex);
						if (type == 2)
							D3DXCreateTextureFromFileEx(gameDevice,L"enemy2Up.png",50,50,0,0,D3DFMT_UNKNOWN,
														D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
														D3DCOLOR_XRGB(255,0,255),&characterTex_I,0,
														&characterTex);
						return true;
					}
				}
			}
		}
	}

	if(direction == 1)
	{
		grid[(aIPos[X])-1][(aIPos[Y])-1] = 0;
		aIPos[Y] -= 1;
		grid[(aIPos[X])-1][(aIPos[Y])-1] = 2;
		kinematic.velocity = Vector3D(0,-100,0);
	}
	if(direction == 2)
	{
		grid[(aIPos[X])-1][(aIPos[Y])-1] = 0;
		aIPos[X] += 1;
		grid[(aIPos[X])-1][(aIPos[Y])-1] = 2;
		kinematic.velocity = Vector3D(100,0,0);
	}
	if(direction == 3)
	{
		grid[(aIPos[X])-1][(aIPos[Y])-1] = 0;
		aIPos[Y] += 1;
		grid[(aIPos[X])-1][(aIPos[Y])-1] = 2;
		kinematic.velocity = Vector3D(0,100,0);
	}
	if(direction == 4)
	{
		grid[(aIPos[X])-1][(aIPos[Y])-1] = 0;
		aIPos[X] -= 1;
		grid[(aIPos[X])-1][(aIPos[Y])-1] = 2;
		kinematic.velocity = Vector3D(-100,0,0);
	}
	return false;
}

bool AIUnit::Update(Scalar delta_t, int aIPos[2])	//Updates the values for the character, with delta_t as
									//the amount of time passed since the last update and
{
	bool stillMoving = true;

	if (dead) //If the ai is supposed to die
	{
		return false;
	}

	Vector3D currentPos = kinematic.position; //the current position of the character
	kinematic.Update(steering,delta_t); //Update the physics based on the steering

	if (direction == 0)
	{
		stillMoving = false;
	}
	if (direction == 1)
	{
		if (kinematic.position[Y] < gridSize * aIPos[Y] - gridSize / 2.0)
		{
			kinematic.position[Y] = gridSize * aIPos[Y] - gridSize / 2.0;
			kinematic.velocity = Vector3D::Zero_Vector();
			direction = 0;
			stillMoving = false;
		}
	}
	if (direction == 2)
	{
		if (kinematic.position[X] > gridSize * aIPos[X] - gridSize / 2.0)
		{
			kinematic.position[X] = gridSize * aIPos[X] - gridSize / 2.0;
			kinematic.velocity = Vector3D::Zero_Vector();
			direction = 0;
			stillMoving = false;
		}
	}
	if (direction == 3)
	{
		if (kinematic.position[Y] > gridSize * aIPos[Y] - gridSize / 2.0)
		{
			kinematic.position[Y] = gridSize * aIPos[Y] - gridSize / 2.0;
			kinematic.velocity = Vector3D::Zero_Vector();
			direction = 0;
			stillMoving = false;
		}
	}
	if (direction == 4)
	{
		if (kinematic.position[X] < gridSize * aIPos[X] - gridSize / 2.0)
		{
			kinematic.position[X] = gridSize * aIPos[X] - gridSize / 2.0;
			kinematic.velocity = Vector3D::Zero_Vector();
			direction = 0;
			stillMoving = false;
		}
	}

	Vector3D posDifference = kinematic.position - currentPos; //find the difference between the current position and the next position
	
	//Keep the character inside the game
	if (kinematic.position[X] < 25)
		kinematic.position[X] = 25;
	if (kinematic.position[X] > windowSize[X] - 25)
		kinematic.position[X] = windowSize[X] - 25;
	if (kinematic.position[Y] < 25)
		kinematic.position[Y] = 25;
	if (kinematic.position[Y] > windowSize[Y] - 25)
		kinematic.position[Y] = windowSize[Y] - 25;

	characterTex_Set.transMat._41 = kinematic.position[X];
	characterTex_Set.transMat._42 = kinematic.position[Y];

	return stillMoving;
}

void AIUnit::Kill(int grid[10][10], int aIPos[2])
{
	dead = true;
	grid[aIPos[X]-1][aIPos[Y]-1] = 0;
	aIPos[X] = -1;
	aIPos[Y] = -1;
}

void AIUnit::Pulse(LPDIRECT3DDEVICE9 gameDevice, float pulseCount)
{
	if (dead)
		return;
	if (count == 0)
	{
		if (look == 1)
			D3DXMatrixScaling(&characterTex_Set.scaleMat,0.8+(pulseCount*0.02),0.8+(pulseCount*0.02),1);
		if (look == 2)
			D3DXMatrixScaling(&characterTex_Set.scaleMat,0.8+(pulseCount*0.02),0.8+(pulseCount*0.02),1);
		if (look == 3)
			D3DXMatrixScaling(&characterTex_Set.scaleMat,0.8+(pulseCount*0.02),0.8+(pulseCount*0.02),1);
		if (look == 4)
			D3DXMatrixScaling(&characterTex_Set.scaleMat,0.8+(pulseCount*0.02),0.8+(pulseCount*0.02),1);
	}
}