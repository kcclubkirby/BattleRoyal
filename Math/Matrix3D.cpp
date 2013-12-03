
#include <assert.h>
#include "Matrix3D.h"

Matrix3D::Matrix3D(void)
{
	for ( int row = 0 ; row < dimension ; row++ )
		for ( int col = 0 ; col < dimension ; col++ )
			m[row][col] = (Scalar)0.0 ;
}

Matrix3D:: Matrix3D( Vector3D Bx, Vector3D By, Vector3D Bz )
{
	for ( int row = 0 ; row < dimension ; row++ ) m[row][X] = Bx[row] ;
	for ( int row = 0 ; row < dimension ; row++ ) m[row][Y] = By[row] ;
	for ( int row = 0 ; row < dimension ; row++ ) m[row][Z] = Bz[row] ;
}

Vector3D Matrix3D:: Basis( int column )
{
	Vector3D result ;

	for ( int row = 0 ; row < dimension ; row++ )
		result[row] = m[row][column] ;

	return result ;
}


void Matrix3D::Identity()
{
	for ( int row = 0 ; row < dimension ; row++ )
		for ( int col = 0 ; col < dimension ; col++ )
			m[row][col] = ( row == col ) ? (Scalar)1.0 : (Scalar)0.0 ;
}

void Matrix3D::Scaling( Scalar uniform_scale )
{
	Identity() ;
	for ( int i = 0 ; i < dimension ; i++ ) m[i][i] = uniform_scale ;
}

void Matrix3D::Scaling( Vector3D scaling )
{
	Identity() ;
	for ( int i = 0 ; i < dimension ; i++ ) m[i][i] = scaling[i] ;
}

void Matrix3D::Scale( Scalar uniform_scale )
{
	Matrix3D scaling_matrix ;
	scaling_matrix.Scaling( Vector3D( uniform_scale, uniform_scale, uniform_scale ) ) ;
	*this *= scaling_matrix ;
}

void Matrix3D::Scale( Vector3D scaling )
{
	Matrix3D scaling_matrix ;
	scaling_matrix.Scaling( scaling ) ;
	*this *= scaling_matrix ;
}

void Matrix3D::Rotation( double radians, int axis )
{
	Scalar cos = (Scalar) Math::cos( radians ) ;
	Scalar sin = (Scalar) Math::sin( radians ) ;

	Identity() ;  // really needed for 3D

	switch ( axis )
	{
		case X:
			m[Y][Y] =  cos ;
			m[Y][Z] = -sin ;
			m[Z][Y] =  sin ;
			m[Z][Z] =  cos ;
			break ;

		case Y:
			m[Z][Z] =  cos ;
			m[Z][X] = -sin ;
			m[X][Z] =  sin ;
			m[X][X] =  cos ;
			break ;

		case Z:
			m[X][X] =  cos ;
			m[X][Y] = -sin ;
			m[Y][X] =  sin ;
			m[Y][Y] =  cos ;
			break ;
		default:
			assert( false ) ;
			break ;
	}
}

void Matrix3D::Rotate( double angle, int axis )
{
	Matrix3D rotation_matrix ;
	rotation_matrix.Rotation( angle, axis ) ;
	*this *= rotation_matrix ;
}

void Matrix3D::Shearing( Vector2D shearing, int axis )
{
	Identity() ;
	switch ( axis )
	{
		case Z:
			m[X][Z] = shearing[0] ;
			m[Y][Z] = shearing[1] ;
			break ;
		case Y:
			m[Z][Y] = shearing[0] ;
			m[X][Y] = shearing[1] ;
			break ;
		case X:
			m[Y][X] = shearing[0] ;
			m[Z][X] = shearing[1] ;
			break ;
		default:
			assert( false ) ;
			break ;
	}
}

void Matrix3D::Shear( Vector2D shear, int axis )
{
	Matrix3D shearing_matrix ;
	shearing_matrix.Shearing( shear, axis ) ;
	*this *= shearing_matrix ;
}

Matrix3D Matrix3D:: operator + ( const Matrix3D& other ) const
{
	Matrix3D result ;

	for ( int row = 0 ; row < dimension ; row++ )
		for ( int col = 0 ; col < dimension ; col++ )
			result.m[row][col] = m[row][col] + other.m[row][col] ;

	return result ;
}

Matrix3D Matrix3D:: operator - ( const Matrix3D& other ) const
{
	Matrix3D result ;

	for ( int row = 0 ; row < dimension ; row++ )
		for ( int col = 0 ; col < dimension ; col++ )
			result.m[row][col] = m[row][col] - other.m[row][col] ;

	return result ;
}

Matrix3D Matrix3D:: operator * ( const Matrix3D& other ) const
{
	Matrix3D result ;

	for ( int row = 0 ; row < dimension ; row++ )
		for ( int col = 0 ; col < dimension ; col++ )
			for ( int k = 0 ; k < dimension ; k++ )
				result.m[row][col] += other.m[row][k] * m[k][col] ;

	return result ;
}

void Matrix3D:: operator *= ( const Matrix3D& other )
{
	Matrix3D tmp ;

	for ( int row = 0 ; row < dimension ; row++ )
		for ( int col = 0 ; col < dimension ; col++ )
			for ( int k = 0 ; k < dimension ; k++ )
				tmp.m[row][col] += other.m[row][k] * m[k][col] ;

	*this = tmp ;
}

Vector3D Matrix3D:: operator * ( const Vector3D& vector ) const
{
	Vector3D result ;

	for ( int row = 0 ; row < dimension ; row++ )
		for ( int col = 0 ; col < dimension ; col++ )
			result[row] += m[row][col] * vector[col] ;

	return result ;
}

void operator *= ( Vector3D& vector, const Matrix3D& matrix )
{
	Vector3D tmp ;

	for ( int row = 0 ; row < matrix.dimension ; row++ )
		for ( int col = 0 ; col < matrix.dimension ; col++ )
			tmp[row] += matrix.m[row][col] * vector[col] ;

	vector = tmp ;
}

Scalar Matrix3D:: Determinant2D( int row, int col ) const
{
	Scalar result ;

	int row1, row2, col1, col2 ;
	row1 = (row+1)%dimension ;
	row2 = (row+2)%dimension ;
	col1 = (col+1)%dimension ;
	col2 = (col+2)%dimension ;

	result = m[row1][col1] * m[row2][col2] - m[row1][col2] * m[row2][col1] ;

	return result ;
}

Scalar Matrix3D:: Determinant() const
{
	Scalar result = 0 ;

	for ( int col = 0 ; col < dimension ; col++ )
		result += m[0][col] * Determinant2D( 0, col ) ;

	return result ;
}


Matrix3D Matrix3D::Inverse() 
{
	Matrix3D result ;

	Scalar one_over_determinant = 1.0f / Determinant() ;

	for ( int row = 0 ; row < dimension ; row++ )
		for ( int col = 0 ; col < dimension ; col++ )
			result.m[col][row] = Determinant2D( row, col ) * one_over_determinant ;

	return result ;
}

void Matrix3D::Invert()
{
	Matrix3D inverse ;

	inverse = Inverse() ;

	*this = inverse ;
}

Matrix3D Matrix3D:: Transpose() const
{
	Matrix3D result ;

	for ( int row = 0 ; row < dimension ; row++ )
		for ( int col = 0 ; col < dimension ; col++ )
			result.m[col][row] = m[row][col] ;

	return result ;
}

Scalar Matrix3D:: Trace() const
{
	Scalar result = 0.0 ;

	for ( int i = 0 ; i < dimension ; i++ ) result += m[i][i] ;

	return result ;
}

bool Matrix3D:: operator == ( const Matrix3D& other ) const
{
	bool result = true ;

	for ( int i = 0 ; i < dimension ; i++ )
		for ( int j = 0 ; j < dimension ; j++ )
			result &= m[i][j] == other.m[i][j] ;
	return result ;
}

bool Matrix3D:: operator != ( const Matrix3D& other ) const
{
	return ! (*this == other) ;
}

const Matrix3D& Matrix3D:: Identity_Matrix()
{
	static Matrix3D identity_matrix ;
	static bool identity_initialize = false ;

	if ( !identity_initialize ) { identity_matrix.Identity() ; identity_initialize = true ; }

	return identity_matrix ;
}


