#include "Quaternion.h"

Quaternion:: Quaternion(void)
{
	v[0] = 1 ; // form identity quaternion
}

Quaternion:: Quaternion( Scalar w, const Vector3D& vector )
{
	v[0] = w ;
	v[1] = vector[0] ;
	v[2] = vector[1] ;
	v[3] = vector[2] ;
}

Quaternion:: Quaternion( const Vector4D& vector )
{
	v[0] = vector[0] ;
	v[1] = vector[1] ;
	v[2] = vector[2] ;
	v[3] = vector[3] ;
}

Quaternion:: Quaternion( Vector3D axis, double angle, bool normalized )
{
	double half_angle = angle * 0.5 ;
	Scalar axis_scale = Scalar( Math::sin( half_angle ) ) ;
	if ( !normalized )axis_scale /= axis.Length() ;

	v[0]  = Scalar( Math::cos( half_angle ) ) ;
	v[1] = axis[0] * axis_scale ;
	v[2] = axis[1] * axis_scale ;
	v[3] = axis[2] * axis_scale ;
}

Quaternion Quaternion:: operator * ( const Quaternion& q2 ) const
{
	const Quaternion& q1 = *this ;

	Vector3D v1( q1.v[1], q1.v[2], q1.v[3] ) ;
	Vector3D v2( q2.v[1], q2.v[2], q2.v[3] ) ;

	Scalar w   = q1.v[0] * q2.v[0] - v1 * v2 ;
	Vector3D v = q1.v[0] * v2 + q2.v[0] * v1 + v2.Cross_Product( v1 ) ;

	Quaternion result( w, v ) ;

	return result ;
}

void Quaternion:: Identity()
{
	v[0] = 1 ;
	v[1] = 0 ;
	v[2] = 0 ;
	v[3] = 0 ;
}

void Quaternion:: operator = ( const Vector4D& vector ) 	// for use when inheriting an operator from Vector4D whose return type is Vector4D
{
	v[0] = vector[0] ;
	v[1] = vector[1] ;
	v[2] = vector[2] ;
	v[3] = vector[3] ;
}

void Quaternion:: operator += ( const Vector3D& vector3D )
{
	*this = *this + Quaternion( 0, vector3D ) * (*this) ;
	this->Normalize() ;
}

void Maxtix4D_to_Matrix3D( Scalar* matrix4D, Scalar* matrix3D )
{
	for ( int row = 0 ; row < 3 ; row++ )
	{
		for ( int col = 0 ; col < 3 ; col++ )
		{
			if ( Math::fabs( *matrix4D) < 10e-6 ) *matrix3D = 0 ;
			else if ( Math::fabs( *matrix4D ) > .999999 ) *matrix3D = *matrix4D > 0 ? 1.0f : -1.0f ;
			else *matrix3D = *matrix4D ;
			matrix3D++, matrix4D++ ;
		}
		matrix4D++ ;
	}
}

void Quaternion:: To_Matrix3D( Matrix3D& matrix3D ) const
{
	Matrix4D matrix4D ;
	To_Matrix4D( matrix4D ) ;
	Maxtix4D_to_Matrix3D( (Scalar*) &matrix4D, (Scalar*) &matrix3D ) ;
}

void Quaternion:: To_Matrix4D( Matrix4D& matrix ) const
{
	Scalar x, y, z, w ;
	w = v[0], x = v[1], y = v[2], z = v[3] ; 
	Matrix4D m1( Vector4D( w, z, -y, -x ), Vector4D( -z, w, x, -y ), Vector4D( y, -x, w, -z ), Vector4D(  x,  y,  z, w ) ) ;
	Matrix4D m2( Vector4D( w, z, -y,  x ), Vector4D( -z, w, x,  y ), Vector4D( y, -x, w,  z ), Vector4D( -x, -y, -z, w ) ) ;
	matrix = m1 * m2 ;
}