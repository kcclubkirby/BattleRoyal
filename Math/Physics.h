#pragma once

#include "Vector4D.h"
#include "Matrix4D.h"
#include "Matrix3D.h"
#include "Quaternion.h"

class InertiaTensor
{
public:
	Matrix3D matrix, inverse ;
    InertiaTensor() ;
    InertiaTensor( double Ixx, double Iyy, double Izz ) ;
    void Scale( const Matrix3D& scaling ) ;
} ;

class Physics
{
public:
	Scalar   mass, inverse_mass, maxAcc, maxVel ;
	Vector3D position ;
	Vector3D velocity ;
	Vector3D acceleration ;
	Vector3D momentum ;
	Vector3D force ;
	Vector3D gravity ;
	InertiaTensor inertia_tensor;
	Vector3D angular_velocity ;
    Vector3D angular_acceleration ;
    Vector3D angular_momentum ;
    Vector3D torque ;// "angular force"
    Quaternion rotation ;
	Vector3D center_of_mass;

	struct { Scalar friction, restitution, drag ; } coefficient ;

public:
	Physics(void);
   ~Physics(void);

	void Set_Mass( double ) ;

	Scalar Speed( void ) ;	// for convenience

	Vector3D dX( double dt ) ;
	Vector3D dW( Scalar dt ) ;
};


class PhysicsObject : public Physics
{
private:
	Matrix4D matrix, inverse ;
	Matrix3D scaling, rotation ;
	Vector3D translation ;
	bool up_to_date ;

public:
	Physics physics ;
	bool in_motion ;

	PhysicsObject() ;

	Matrix4D& Matrix() ;
	Matrix4D& Inverse() ;
	Matrix3D& Scaling() ;
	void Scale3D( Vector3D scale ) ;
	void Rotate3D( double angle, int axis ) ;
	void Translate3D( Vector3D translate ) ;

	void Update( float delta_t ) ;

	static Vector3D Reflected_Direction( Vector3D incident, Vector3D normal ) ;
	friend class Contact ;

	Vector3D Translation() {return translation;}; //added this so we can call the translation vector in Crate
} ;	// PhysicsObject


class Contact
{
public:
    Vector3D point ; //point of contact
    Vector3D normal ; // contact normal
    PhysicsObject* object[2] ; // two object that are possibly colliding
    Scalar COR ;// Coefficient of Restitution
    Scalar penetration ; // Penetration in the collision
    bool collided ; // did these two objects collide??
	
	Contact() ;
	void Compute_Response() ;
} ;

