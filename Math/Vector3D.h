#pragma once

#include "Scalar.h"

class Vector3D
{
public:

	Vector3D();
	Vector3D( const Vector3D& ) ;	// copy constructor
	Vector3D( Scalar element[3] ) ; // initialize with an array
	Vector3D( double x, double y, double z ) ;

	Scalar& operator [] ( int index ) ;			// use to both read and write elements, just like a normal array
	Scalar  operator [] ( int index ) const ;	// use to read elements from const vectors

	Scalar Length() const ; 
	void   Normalize() ;
	Vector3D Normalized() const ; // return a new unit vector

	// Scalar-Vector product
	Vector3D operator  - ( void ) const ; // negate a vector
	Vector3D operator * ( Scalar ) const ; // for the case when the operand order is Vector * Scalar
friend
	Vector3D operator * ( Scalar, Vector3D& ) ; // for the case when the operand order is Scalar * Vector
	Vector3D operator /  ( Scalar ) ;
	void     operator *= ( Scalar ) ;
	void     operator /= ( Scalar ) ;

	// vector addition
	Vector3D operator +  ( Vector3D& ) const ;
	Vector3D operator -  ( Vector3D& ) const ;
	void     operator += ( Vector3D& ) ;
	void     operator -= ( Vector3D& ) ;

	// the Dot-Product
	Scalar   operator *  ( Vector3D& ) const ;
	// Construct a vector that is orthogonal (perpendicular) to the given vector
	Vector3D Cross_Product( const Vector3D& other ) const ;
	Vector3D operator %   ( const Vector3D& ) const ;	// overload for cross product

	bool     operator == ( const Vector3D& ) const ;
	bool     operator != ( const Vector3D& ) const ;

	// use this to test whethere or not a vector == zero vector
	// it returns a reference to a constant static vector full of 0's
	static const Vector3D& Zero_Vector() ;


protected:
	static const int dimension = 3 ;
	Scalar v[dimension] ;
};
