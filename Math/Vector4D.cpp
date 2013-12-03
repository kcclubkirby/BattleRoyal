#pragma once

#include "Vector4D.h"
#include "assert.h"



Vector4D& Vector4D:: Zero_Vector() 
{
	static Vector4D zero_vector ;

	return zero_vector ;
}

Vector4D:: Vector4D()
{
	for ( int i = 0 ; i < dimension ; i++ ) v[i] = 0.0f ;
}

Vector4D:: Vector4D( const Vector4D& other )
{
	*this = other ;
}

Vector4D:: Vector4D( Scalar element[4] )
{
	for ( int i = 0 ; i < dimension ; i++ )
		v[i] = element[i] ;
}

Vector4D:: Vector4D( double x, double y, double z, double w )
{
	v[X] = (Scalar) x ;
	v[Y] = (Scalar) y ;
	v[Z] = (Scalar) z ;
	v[W] = (Scalar) w ;
}

Vector4D:: Vector4D( double x, double y, double z )
{
	v[X] = (Scalar) x ;
	v[Y] = (Scalar) y ;
	v[Z] = (Scalar) z ;
	v[W] = (Scalar) 1.0 ;
}

Vector4D:: Vector4D( Vector3D v3d, Scalar w )
{
	v[X] = v3d[X] ;
	v[Y] = v3d[Y] ;
	v[Z] = v3d[Z] ;
	v[W] = w ;
}

Vector3D Vector4D:: ToVector3D()			// returns the 1st 3 components as a Vector3D
{
	return Vector3D( v[X], v[Y], v[Z] ) ;
}

Scalar& Vector4D::operator []( int index )
{
	assert( index >= 0 && index < 4 ) ;
	return v[index] ;
}

Scalar Vector4D::operator []( int index ) const
{
	assert( index >= 0 && index < 4 ) ;
	return v[index] ;
}

Scalar Vector4D::Length() const
{
	Scalar length = 0.0 ;
	for ( int i = 0 ; i < dimension ; i++ ) length += v[i] * v[i] ;
	length = Math::sqrt( length ) ;
	return length ;
}


Scalar Vector4D::Length3D() const
{
	Scalar length = 0.0 ;
	for ( int i = 0 ; i < 3 ; i++ ) length += v[i] * v[i] ;
	length = Math::sqrt( length ) ;
	return length ;
}

void Vector4D::Normalize()
{
	Scalar length = Length() ;
	for ( int i = 0 ; i < dimension ; i++ )
		v[i] /= length ;
}

void Vector4D::Normalize3D()
{
	Scalar one_over_length = Scalar( 1.0 ) / Length3D() ;
	for ( int i = 0 ; i < dimension ; i++ )
		v[i] *= one_over_length ;
}

Vector4D Vector4D::Normalized() const
{
	Vector4D result = *this ;
	result /= Length() ;
	return result ;
}

void Vector4D::operator *= ( Scalar scalar )
{
	for ( int i = 0 ; i < dimension ; i++ )
		v[i] *=  scalar ;
}

Vector4D Vector4D::operator * ( Scalar scalar ) const
{
	Vector4D result ;
	for ( int i = 0 ; i < dimension ; i++ )
		result.v[i] =  v[i] * scalar ;
	return result ;
}

Vector4D operator * ( Scalar scalar, const Vector4D& vector )
{
	Vector4D result ;
	for ( int i = 0 ; i < result.dimension ; i++ )
		result.v[i] =  scalar * vector.v[i] ;
	return result ;
}

void Vector4D::operator /= ( Scalar scalar )
{
	for ( int i = 0 ; i < dimension ; i++ )
		v[i] /=  scalar ;
}

Vector4D Vector4D::operator / ( Scalar scalar )
{
	Vector4D result ;
	for ( int i = 0 ; i < dimension ; i++ )
		result.v[i] =  v[i] / scalar ;
	return result ;
}



void Vector4D::operator += ( const Vector4D& other )
{
	for ( int i = 0 ; i < dimension ; i++ )
		v[i] += other.v[i] ;
}

void Vector4D::operator -= ( const Vector4D& other )
{
	for ( int i = 0 ; i < dimension ; i++ )
		v[i] -= other.v[i] ;
}

Vector4D Vector4D::operator + ( const Vector4D& other ) const
{
	Vector4D result ;
	for ( int i = 0 ; i < dimension ; i++ )
		result.v[i] = v[i] + other.v[i] ;
	return result ;
}

Vector4D Vector4D::operator - ( const Vector4D& other ) const
{
	Vector4D result ;
	for ( int i = 0 ; i < dimension ; i++ )
		result.v[i] = v[i] - other.v[i] ;
	return result ;
}

Scalar Vector4D::operator * ( const Vector4D& other ) const
{	// Dor Product
	Scalar dot_product = 0.0 ;
	for ( int i = 0 ; i < dimension ; i++ )
		dot_product += v[i] * other.v[i] ;
	return dot_product ;
}

Vector4D Vector4D::operator - ( void ) const
{
	Vector4D result ;
	for ( int i = 0 ; i < dimension ; i++ )
		result.v[i] = -v[i] ;
	return result ;
}

Vector4D Vector4D::Cross_Product( const Vector4D& other ) const
{
	Vector4D result ;

	result.v[X] = v[Y] * other.v[Z] - other.v[Y] * v[Z] ;
	result.v[Y] = v[Z] * other.v[X] - other.v[Z] * v[X] ;
	result.v[Z] = v[X] * other.v[Y] - other.v[X] * v[Y] ;

	return result ;
}

Vector4D Vector4D::operator % ( const Vector4D& other ) const
{
	return Cross_Product( other ) ;
}

Vector3D Vector4D::operator % ( const Vector3D& other ) const
{
	Vector3D vector = Vector3D( v[X], v[Y], v[Z] ) ;

	return vector.Cross_Product( other ) ;
}

Vector4D Cross_Product( Vector4D v1, Vector4D v2 )
{
	Vector4D result ;
	Vector4D V1( v1 ), V2( v2 ) ;

	result.v[0] = v1.v[1] * v2.v[2] - v1.v[2] * v2.v[1] ;
	result.v[1] = v1.v[2] * v2.v[0] - v1.v[0] * v2.v[2] ;
	result.v[2] = v1.v[0] * v2.v[1] - v1.v[1] * v2.v[0] ;

	return result ;
}



bool Vector4D:: operator == ( const Vector4D& other ) const
{
	bool result = true ;

	for ( int i = 0 ; i < dimension ; i++ )
		result &= v[i] == other.v[i] ;
	return result ;
}

bool Vector4D:: operator != ( const Vector4D& other ) const
{
	return ! (*this == other) ;
}
