
#include "Vector2D.h"


Vector2D:: Vector2D()
{
	for ( int i = 0 ; i < dimension ; i++ ) v[i] = 0.0f ;
}

Vector2D:: Vector2D( const Vector2D& other )
{
	*this = other ;
}

Vector2D:: Vector2D( double x, double y )
{
	v[X] = Scalar( x ) ;
	v[Y] = Scalar( y ) ;
}

Vector2D:: Vector2D( Scalar element[2] )
{
	for ( int i = 0 ; i < dimension ; i++ )
		v[i] = element[i] ;
}

Scalar& Vector2D::operator []( int index )
{
	return v[index] ;
}

Scalar Vector2D::operator []( int index ) const
{
	return v[index] ;
}

Scalar Vector2D::Length()
{
	Scalar length = 0.0 ;
	for ( int i = 0 ; i < dimension ; i++ ) length += v[i] * v[i] ;
	return Math::sqrt( length ) ;
}

void Vector2D::Normalize()
{
	Scalar length = Length() ;
	if ( length > epsilon )	// do not divide is length is really small
		for ( int i = 0 ; i < dimension ; i++ )
			v[i] /= length ;
}


Vector2D Vector2D::Normalized() const
{
	Vector2D result( *this ) ;
	result.Normalize() ;
	return result ;
}

Vector2D Vector2D::operator - ( void ) const
{
	Vector2D result = *this ;
	for ( int i = 0 ; i < dimension ; i++ )
		result.v[i] *=  Scalar( -1.0 ) ;
	return result ;
}

void Vector2D::operator *= ( Scalar scalar )
{
	for ( int i = 0 ; i < dimension ; i++ )
		v[i] *=  scalar ;
}

void Vector2D::operator /= ( Scalar scalar )
{
	for ( int i = 0 ; i < dimension ; i++ )
		v[i] /=  scalar ;
}


Vector2D Vector2D::operator * ( Scalar scalar ) const
{
	Vector2D result ;
	for ( int i = 0 ; i < dimension ; i++ )
		result.v[i] =  v[i] * scalar ;
	return result ;
}

Vector2D Vector2D::operator / ( Scalar scalar ) const
{
	Vector2D result ;
	for ( int i = 0 ; i < dimension ; i++ )
		result.v[i] =  v[i] / scalar ;
	return result ;
}

// friend
Vector2D operator * ( Scalar scalar, Vector2D& vector )
{
	Vector2D result ;
	for ( int i = 0 ; i < result.dimension ; i++ )
		result.v[i] =  scalar * vector.v[i] ;
	return result ;
}

void Vector2D::operator += ( Vector2D& other )
{
	for ( int i = 0 ; i < dimension ; i++ )
		v[i] += other.v[i] ;
}

void Vector2D::operator -= ( Vector2D& other )
{
	for ( int i = 0 ; i < dimension ; i++ )
		v[i] -= other.v[i] ;
}

Vector2D Vector2D::operator + ( Vector2D& other ) const
{
	Vector2D result ;
	for ( int i = 0 ; i < dimension ; i++ )
		result.v[i] = v[i] + other.v[i] ;
	return result ;
}

Vector2D Vector2D::operator - ( Vector2D& other ) const
{
	Vector2D result ;
	for ( int i = 0 ; i < dimension ; i++ )
		result.v[i] = v[i] - other.v[i] ;
	return result ;
}

Scalar Vector2D::operator * ( Vector2D& other ) const
{
	Scalar dot_product = 0.0 ;
	for ( int i = 0 ; i < dimension ; i++ )
		dot_product += v[i] * other.v[i] ;
	return dot_product ;
}

bool Vector2D:: operator == ( const Vector2D& other ) const
{
	bool result = true ;

	for ( int i = 0 ; i < dimension ; i++ )
		result &= v[i] == other.v[i] ;
	return result ;
}

bool Vector2D:: operator != ( const Vector2D& other ) const
{
	return ! (*this == other) ;
}

Vector2D Vector2D:: Orthogonal() const
{
	Vector2D result ;

	result.v[X] = -v[Y] ;
	result.v[Y] =  v[X] ;

	return result ;
}

const Vector2D& Vector2D:: Zero_Vector() 
{
	static Vector2D zero_vector ;

	return zero_vector ;
}



