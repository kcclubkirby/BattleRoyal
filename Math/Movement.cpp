/*------------------------------------------------------*
*CREATED BY:											*
*Name: David Tiscareno									*
*Date: 3/10/13											*
*Movement.cpp											*
*This file contains the implementation of the classes	*
*from the book:	Location, Kinematic, and SteeringOutput.*
*-------------------------------------------------------*/

#include "Movement.h"

SteeringOutput::SteeringOutput() //Default constructor
{
	linear = Vector3D::Zero_Vector();
	angular = 0.0;

	maxLinAcceleration = 100;
	maxAngAcceleration = 3;
}

SteeringOutput::SteeringOutput(Vector3D lin, Scalar ang) //Specific constructor
{
	linear = lin;
	angular = ang;

	maxLinAcceleration = 100;
	maxAngAcceleration = 3;
}

SteeringOutput SteeringOutput::operator* (Scalar num) //Allows for the steering behavior to be modified by multiplication
{
	SteeringOutput steering;
	
	//multiplies the accelerations by the number
	steering.linear = linear * num;
	steering.angular = angular * num;

	return steering;
}

SteeringOutput SteeringOutput::operator+ (SteeringOutput steerer)
{
	SteeringOutput steering;
	
	//adds a steering behavior to another steering behavior
	steering.linear = linear + steerer.linear;
	steering.angular = angular + steerer.angular;

	return steering;
}

void SteeringOutput::operator+= (SteeringOutput steerer)
{
	//adds a steering behavior to this steering behavior
	linear = linear + steerer.linear;
	angular = angular + steerer.angular;
}

Location::Location(Vector3D pos, Scalar orient) //Specific constructor
{
	position = pos;
	orientation = orient;

	direction = Vector3D(1*cos(orientation),1*sin(orientation),0);
}

Kinematic::Kinematic() //Default constructor
{
	position = Vector3D::Zero_Vector();
	orientation = 0.0;
	velocity = Vector3D::Zero_Vector();
	rotation = 0.0;

	maxSpeed = 200;
	maxRotation = 6;
}

Kinematic::Kinematic(Vector3D pos, Scalar orient, Vector3D vel, Scalar rot) //Specific constructor
{
	position = pos;
	orientation = orient;
	velocity = vel;
	rotation = rot;

	maxSpeed = 200;
	maxRotation = 6;

	direction = Vector3D(1*cos(orientation),1*sin(orientation),0);
}

void Kinematic::Update(SteeringOutput steering, Scalar delta_t) //Updates the physics for an object based on an acceleration and time
{
	//first make sure the accelerations are equal to or less than the maximum
	if (steering.linear.Length() > steering.maxLinAcceleration)
		steering.linear = steering.linear.Normalized() * steering.maxLinAcceleration;
	if (abs(steering.angular) > steering.maxAngAcceleration)
	{
		if (steering.angular > 0)
			steering.angular = steering.maxAngAcceleration;
		else
			steering.angular = -steering.maxAngAcceleration;
	}

	//update the position and orientations
	position += velocity * delta_t + 0.5 * steering.linear * delta_t * delta_t ;
	orientation += rotation * delta_t + 0.5 * steering.angular * delta_t * delta_t ;

	//Make sure the orientation is between pi and -pi
	if (orientation > Math::PI)
	{
		orientation -= 2 * Math::PI;
	}
	else if (orientation < -Math::PI)
	{
		orientation += 2 * Math::PI;
	}

	//Update the velocity and rotations
	velocity += steering.linear * delta_t;
	rotation += steering.angular * delta_t;

	//make sure the velocity and rotations are less than or equal to the maximum
	if (velocity.Length() > maxSpeed)
		velocity = velocity.Normalized() * maxSpeed;
	if (abs(rotation) > maxRotation)
	{
		if (rotation > 0)
			rotation = maxRotation;
		else
			rotation = -maxRotation;
	}

	//update the direction vector
	direction = Vector3D(1*cos(orientation),1*sin(orientation),0);
}