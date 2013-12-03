#pragma once

#include "Scalar.h"
#include "Vector3D.h"

class Vector4D
{
public:

	Vector4D() ;
	Vector4D( const Vector4D& ) ;	// copy constructor
	Vector4D( Scalar element[4] ) ; // initialize with an array
	Vector4D( double x, double y, double z, double w ) ;
	Vector4D( double x, double y, double z ) ;  // w == 1.0 is implicit
	Vector4D( Vector3D, Scalar w ) ; // w == 0.0 or 1.0

	Vector3D ToVector3D() ;			// returns the 1st 3 components as a Vector3D
	Scalar& operator [] ( int index ) ;			// use to both read and write elements, just like a normal array
	Scalar  operator [] ( int index ) const ;	// use to read elements from a const vector


	Scalar Length() const ;
	Scalar Length3D() const ; // of the normal
	void   Normalize() ;
	void   Normalize3D() ; // the normal only
	Vector4D Normalized() const ; // return a new unit vector


	void     operator *= ( Scalar ) ;
	Vector4D operator *  ( Scalar ) const ;
	friend Vector4D operator * ( Scalar, const Vector4D& ) ;
	void     operator /= ( Scalar ) ;
	Vector4D operator /  ( Scalar ) ;


	void     operator += ( const Vector4D& ) ;
	void     operator -= ( const Vector4D& ) ;
	Vector4D operator +  ( const Vector4D& ) const ;
	Vector4D operator -  ( const Vector4D& ) const ;
	Scalar   operator *  ( const Vector4D& ) const ;	// the DOT PRODUCT
	Vector4D operator -  ( void ) const ;	// negative sign

	Vector4D Cross_Product( const Vector4D& ) const ;
	Vector4D operator %   ( const Vector4D& ) const ;	// overload for cross product
	Vector3D operator %   ( const Vector3D& ) const ;	// cross product with a Vector3D
	friend Vector4D Cross_Product( Vector4D, Vector4D ) ;

	bool     operator == ( const Vector4D& ) const ;
	bool     operator != ( const Vector4D& ) const ;

	static Vector4D& Zero_Vector() ;

protected:
	static const int dimension = 4 ;
	Scalar v[dimension] ;
};