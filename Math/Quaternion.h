#pragma once
#include "Vector4D.h"
#include "Matrix4D.h"
#include "Matrix3D.h"

class Quaternion : public Vector4D
{
public:
	Quaternion() ;
	Quaternion( Scalar w, const Vector3D& v ) ;
	Quaternion( const Vector4D& ) ;
	Quaternion( Vector3D axis, double angle_in_radians, bool axis_normalized = false ) ;

	void Identity() ; // set to the indentity quaternion

	Quaternion operator *  ( const Quaternion& ) const ;	// concatenate two quaternions
	void       operator += ( const Vector3D& ) ;	   // for incrementing rotations by the change in angular velocity
	void       operator =  ( const Vector4D& ) ;	// for use when inheriting an operator from Vector4D whose return type is Vector4D

	void To_Matrix3D( Matrix3D& matrix ) const ;
	void To_Matrix4D( Matrix4D& matrix ) const ;
};
