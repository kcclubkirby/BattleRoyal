/*------------------------------------------------------*
*CREATED BY:											*
*Name: David Tiscareno									*
*Date: 3/7/13											*
*Movement.h												*
*This header file contains the classes from the book:	*
*Location, Kinematic, and SteeringOutput. These classes	*
*are implemented in their own cpp files.				*
*-------------------------------------------------------*/

#ifndef MOVEMENT
#define MOVEMENT

#include <math.h>
#include "Vector3D.h"

class SteeringOutput //This is the class which finds the acceleration for each behavior
{
public:
	Vector3D linear; //Linear acceleration
	Scalar angular; //Angular acceleration

	Scalar maxLinAcceleration;
	Scalar maxAngAcceleration;

	SteeringOutput(); //Default constructor
	SteeringOutput(Vector3D lin, Scalar ang); //Specific constructor

	//Allows for the steering behavior to be modified through math
	SteeringOutput operator* (Scalar num);
	SteeringOutput operator+ (SteeringOutput steerer);
	void operator+= (SteeringOutput steerer);
};

class Location //This class holds the location and orientation of an object
{
public:
	Vector3D position;
	Scalar orientation;
	Vector3D direction;

	Location(){}; //Default constructor
	Location(Vector3D pos, Scalar orient); //Specific constructor
};

class Kinematic : public Location //This class holds the regular physics data for an object
{
public:
	Vector3D velocity;
	Scalar rotation;

	Scalar maxSpeed;
	Scalar maxRotation;

	Kinematic(); //Default constructor
	Kinematic(Vector3D pos, Scalar orient, Vector3D vel, Scalar rot); //Specific constructor

	void Update(SteeringOutput steering, Scalar delta_t); //Updates the physics for an object based on an acceleration and time
};

#endif