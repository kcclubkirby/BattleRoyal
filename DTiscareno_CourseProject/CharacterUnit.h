/*------------------------------------------------------*
*CREATED BY:											*
*Name: David Tiscareno									*
*Class: GSP361											*
*Course Project											*
*Date: 6/26/13											*
*CharacterUnit.h										*
*This header file contains the CharacterUnit class, from*
*which the AIUnit and Player classes are derived. It	*
*also includes the AIUnit and Player class declarations.*
*The implementations of each are in the cpp file with	*
*the name of the corresponding class.					*
*-------------------------------------------------------*/

#ifndef CHARACTER_UNIT
#define CHARACTER_UNIT

#include <d3d9.h>
#include <d3dx9.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "Movement.h"

//This struct will determine the locations, dimensions, and colors of the sprites
struct spriteSetup
{
	D3DXMATRIX transMat, rotMat, scaleMat; //the different matrices
	int r,g,b; //the colors
	float rotator; //the rotation variable

	//randomize the variables
	void Randomize(bool includeRotation)
	{
		//start with identity matrices
		D3DXMatrixIdentity(&transMat);
		D3DXMatrixIdentity(&rotMat);
		D3DXMatrixIdentity(&scaleMat);

		//now randomize the different variables

		float scale = ((float)rand()/((float)RAND_MAX/2.5))+0.5;
		D3DXMatrixScaling(&scaleMat, scale, scale, 0.0f);	// Scaling

		rotator = (float)RAND_MAX/360.0;
		if (includeRotation)
			D3DXMatrixRotationZ(&rotMat, D3DXToRadian((float)rand()/
								(rotator)));	// Rotation on Z axis, value in radians, converting from degrees

		D3DXMatrixTranslation(&transMat, (float)rand()/((float)RAND_MAX/800),
							  (float)rand()/((float)RAND_MAX/600), 0.0f);	// Translation

		r = rand()%255;
		g = rand()%255;
		b = rand()%255;
	}

	//randomize the variables
	void Initialize()
	{
		//start with identity matrices
		D3DXMatrixIdentity(&transMat);
		D3DXMatrixIdentity(&rotMat);
		D3DXMatrixIdentity(&scaleMat);

		r = 255;
		g = 255;
		b = 255;
	}

	//return a worldMat which incorporates the scaling, rotation, and translation
	D3DXMATRIX CreateWorldMat()
	{
		//Create temporary matrices
		D3DXMATRIX worldMat, tempScale, tempRot, tempTrans;
		D3DXMatrixIdentity(&worldMat);

		//set the temporary matrices
		tempScale = scaleMat;
		tempRot = rotMat;
		tempTrans = transMat;

		//perform the matrix math
		D3DXMatrixMultiply(&tempScale, &tempScale, &tempRot);	// Multiply scale and rotation, store in scale
		D3DXMatrixMultiply(&worldMat, &tempScale, &tempTrans);	// Multiply scale and translation, store in world

		return worldMat;
	}
};

class CharacterUnit //This class will be the parent class for all characters in the game
{
public:
	IDirect3DTexture9*	characterTex;
	D3DXIMAGE_INFO		characterTex_I;
	spriteSetup characterTex_Set;
	Kinematic kinematic; //Holds the physics for the character
	SteeringOutput steering; //Holds the steering for the characters
	Vector3D windowSize; //size of the window
	int gridSize; //the size of each grid space
	int direction; //The direction the character is moving

	CharacterUnit(){}; //Default constructor

	void Translate(Vector3D T); //Translates the triangle
	void Update(Scalar delta_t); //Updates all values for the character
	void Update(Scalar delta_t,CharacterUnit& character){}; //Reserved for AIUnit
	void Render(); //Renders the character on the screen
};

class AIUnit : public CharacterUnit //This is the version of the CharacterUnit controlled by AI
{
public:
	int steeringNum; //This number holds the value of the current steering behavior
	bool defeatPlayer; //Did the AI character defeat the player?
	bool dead; //Is the character dead?
	int count; //Counter for certain enemy types
	int type; //The type of enemy
	int look; //Direction the character is facing

	AIUnit() //Default constructor
	{
		dead = true;
		count = -1;
	};
	AIUnit(Vector3D pos, Vector3D winSize, int gridSize, LPDIRECT3DDEVICE9 gameDevice, int enType); //Specific constructor

	bool Move(int grid[10][10], int aIPos[2], int maxGrid, int playerPos[2], LPDIRECT3DDEVICE9 gameDevice); //Moves the character in a direction
	bool Update(Scalar delta_t, int aIPos[2]); //Updates the values for the character
	void Kill(int grid[10][10], int aIPos[2]);
	void Pulse(LPDIRECT3DDEVICE9 gameDevice, float pulseCount);
};

class Player : public CharacterUnit //This is the version of the CharacterUnit controlled by the player
{
public:
	bool exists; //Tests whether the player exists
	bool dying; //Is the player going to die?

	Player() //Default constructor
	{
		exists = false; //The player does not exist
	};

	Player(Vector3D pos, Vector3D winSize, int gridSize, LPDIRECT3DDEVICE9 gameDevice); //Specific constructor

	void checkIfMove(int maxGrid,int playerPos[2],int grid[10][10],int& collision, LPDIRECT3DDEVICE9 gameDevice ); //checks if the player is going to move
	bool Move(int grid[10][10], int playerPos[2]); //Moves the character in a direction
	bool Update(Scalar delta_t, int playerPos[2]); //Updates the values for the character
	void Render(); //Renders the character on the screen
};

#endif