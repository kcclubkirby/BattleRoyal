#pragma once

#include "Scalar.h"

class Vector2D
{
public:

	Vector2D();
	Vector2D( const Vector2D& ) ;	// copy constructor
	Vector2D( Scalar element[2] ) ; // initialize with an array
	Vector2D( double x, double y ) ;

	Scalar& operator [] ( int index ) ;			// use to both read and write elements, just like a normal array
	Scalar  operator [] ( int index ) const ;	// use to read elements from const vectors

	Scalar Length() ;
	void   Normalize() ;	// modifies the vector to be unit length
	Vector2D Normalized() const ; // returns a new vector that is unit length

	// Scalar-Vector product
	Vector2D operator  - ( void ) const ; // return negative of a vector
	void     operator *= ( Scalar ) ;
	void     operator /= ( Scalar ) ;
	Vector2D operator * ( Scalar ) const ; // for the case when the operand order is Vector * Scalar
	Vector2D operator / ( Scalar ) const ; // for the case when the operand order is Vector * Scalar
	friend Vector2D operator * ( Scalar, Vector2D& ) ; // for the case when the operand order is Scalar * Vector

	// vector addition
	Vector2D operator +  ( Vector2D& ) const ;
	Vector2D operator -  ( Vector2D& ) const ;
	void     operator += ( Vector2D& ) ;
	void     operator -= ( Vector2D& ) ;

	// the Dot-Product
	Scalar   operator *  ( Vector2D& ) const ;
	// Construct a vector that is orthogonal (perpendicular) to the given vector
	Vector2D Orthogonal() const ;

	bool     operator == ( const Vector2D& ) const ;
	bool     operator != ( const Vector2D& ) const ;

	// use this to test whethere or not a vector == zero vector
	// it returns a reference to a constant static vector full of 0's
	static const Vector2D& Zero_Vector() ;


protected:
	static const int dimension = 2 ;
	Scalar v[dimension] ;
};
