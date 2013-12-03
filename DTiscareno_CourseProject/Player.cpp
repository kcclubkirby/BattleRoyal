/*------------------------------------------------------*
*CREATED BY:											*
*Name: David Tiscareno									*
*Class: GSP361											*
*Course Project											*
*Date: 6/26/13											*
*Player.cpp												*
*This is the implementation of the Player class.		*
*-------------------------------------------------------*/

#include "CharacterUnit.h"

Player::Player(Vector3D pos, Vector3D winSize, int grSize, LPDIRECT3DDEVICE9 gameDevice )	//This constructor will initialize the values where
														//pos is a Vector for the triangle's position,
{
	exists = true;

	kinematic = Kinematic(pos,0,Vector3D::Zero_Vector(),0); //Initializes the physics for the character

	D3DXCreateTextureFromFileEx(gameDevice,L"PlayerDown.png",50,50,0,0,D3DFMT_UNKNOWN,
								D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
								D3DCOLOR_XRGB(255,0,255),&characterTex_I,0,
								&characterTex);
	characterTex_Set.Initialize();
	characterTex_Set.transMat._41 = pos[X];
	characterTex_Set.transMat._42 = pos[Y];

	steering = SteeringOutput(Vector3D::Zero_Vector(),0.0); //Initializes the steering at zero

	dying = false; //Initialize the booleans

	windowSize = winSize; //set the window size
	gridSize = grSize; //the size of each grid space

	direction = 0;
}

void Player::checkIfMove(int maxGrid,int playerPos[2],int grid[10][10],int& collision, LPDIRECT3DDEVICE9 gameDevice )
{
	collision = -1;
	if (!exists) //if the player doesn't exist, don't do anything
		return;

	//Create the different key states
	short left = GetKeyState(VK_LEFT);
	short right = GetKeyState(VK_RIGHT);
	short up = GetKeyState(VK_UP);
	short down = GetKeyState(VK_DOWN);

	//Check each key to see which is being pressed
	if (up < 0)
	{
		D3DXCreateTextureFromFileEx(gameDevice,L"PlayerUp.png",50,50,0,0,D3DFMT_UNKNOWN,
								D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
								D3DCOLOR_XRGB(255,0,255),&characterTex_I,0,
								&characterTex);
		if (playerPos[Y]-1 == 0)
			direction = 0;
		else
		{
			if (grid[playerPos[X]-1][playerPos[Y]-2] == 0)
			{
				direction = 1;
			}
			else
			{
				direction = 0;
				collision = 1;
			}
		}
	}
	if (right < 0)
	{
		D3DXCreateTextureFromFileEx(gameDevice,L"PlayerRight.png",50,50,0,0,D3DFMT_UNKNOWN,
								D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
								D3DCOLOR_XRGB(255,0,255),&characterTex_I,0,
								&characterTex);
		if (playerPos[X]-1 == maxGrid)
			direction = 0;
		else
		{
			if (grid[playerPos[X]][playerPos[Y]-1] == 0)
			{
				direction = 2;
			}
			else
			{
				direction = 0;
				collision = 2;
			}
		}
	}
	if (down < 0)
	{
		D3DXCreateTextureFromFileEx(gameDevice,L"PlayerDown.png",50,50,0,0,D3DFMT_UNKNOWN,
								D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
								D3DCOLOR_XRGB(255,0,255),&characterTex_I,0,
								&characterTex);
		if (playerPos[Y]-1 == maxGrid)
			direction = 0;
		else
		{
			if (grid[playerPos[X]-1][playerPos[Y]] == 0)
			{
				direction = 3;
			}
			else
			{
				direction = 0;
				collision = 3;
			}
		}
	}
	if (left < 0)
	{
		D3DXCreateTextureFromFileEx(gameDevice,L"PlayerLeft.png",50,50,0,0,D3DFMT_UNKNOWN,
								D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
								D3DCOLOR_XRGB(255,0,255),&characterTex_I,0,
								&characterTex);
		if (playerPos[X]-1 == 0)
			direction = 0;
		else
		{
			if (grid[playerPos[X]-2][playerPos[Y]-1] == 0)
			{
				direction = 4;
			}
			else
			{
				direction = 0;
				collision = 4;
			}
		}
	}
	return;
}

bool Player::Move(int grid[10][10], int playerPos[2])
{
	if (!exists) //if the player doesn't exist, don't do anything
		return false;
	if(direction == 0)
	{
		return false;
	}
	if(direction == 1)
	{
		grid[(playerPos[X])-1][(playerPos[Y])-1] = 0;
		playerPos[Y] -= 1;
		grid[(playerPos[X])-1][(playerPos[Y])-1] = 1;
		kinematic.velocity = Vector3D(0,-100,0);
	}
	if(direction == 2)
	{
		grid[(playerPos[X])-1][(playerPos[Y])-1] = 0;
		playerPos[X] += 1;
		grid[(playerPos[X])-1][(playerPos[Y])-1] = 1;
		kinematic.velocity = Vector3D(100,0,0);
	}
	if(direction == 3)
	{
		grid[(playerPos[X])-1][(playerPos[Y])-1] = 0;
		playerPos[Y] += 1;
		grid[(playerPos[X])-1][(playerPos[Y])-1] = 1;
		kinematic.velocity = Vector3D(0,100,0);
	}
	if(direction == 4)
	{
		grid[(playerPos[X])-1][(playerPos[Y])-1] = 0;
		playerPos[X] -= 1;
		grid[(playerPos[X])-1][(playerPos[Y])-1] = 1;
		kinematic.velocity = Vector3D(-100,0,0);
	}
	return true;
}

bool Player::Update(Scalar delta_t, int playerPos[2])	//Updates the values for the character, with delta_t as
									//the amount of time passed since the last update
{
	bool stillMoving = true;

	if (!exists) //if the player doesn't exist, don't do anything
		return false;

	if (dying) //If the player is supposed to die
	{
		dying = false;
	}

	Vector3D currentPos = kinematic.position; //the current position of the character
	kinematic.Update(steering,delta_t); //Update the physics based on the steering

	if (direction == 0)
	{
		stillMoving = false;
	}
	if (direction == 1)
	{
		if (kinematic.position[Y] < gridSize * playerPos[Y] - gridSize / 2.0)
		{
			kinematic.position[Y] = gridSize * playerPos[Y] - gridSize / 2.0;
			kinematic.velocity = Vector3D::Zero_Vector();
			direction = 0;
			stillMoving = false;
		}
	}
	if (direction == 2)
	{
		if (kinematic.position[X] > gridSize * playerPos[X] - gridSize / 2.0)
		{
			kinematic.position[X] = gridSize * playerPos[X] - gridSize / 2.0;
			kinematic.velocity = Vector3D::Zero_Vector();
			direction = 0;
			stillMoving = false;
		}
	}
	if (direction == 3)
	{
		if (kinematic.position[Y] > gridSize * playerPos[Y] - gridSize / 2.0)
		{
			kinematic.position[Y] = gridSize * playerPos[Y] - gridSize / 2.0;
			kinematic.velocity = Vector3D::Zero_Vector();
			direction = 0;
			stillMoving = false;
		}
	}
	if (direction == 4)
	{
		if (kinematic.position[X] < gridSize * playerPos[X] - gridSize / 2.0)
		{
			kinematic.position[X] = gridSize * playerPos[X] - gridSize / 2.0;
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

void Player::Render() //Renders the character on the screen
{
	if (!exists)
		return;
}