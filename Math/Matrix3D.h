#pragma once

#include "Vector2D.h"
#include "Vector3D.h"

class Matrix3D
{
public:
	Matrix3D(void) ;						// set to the zero matrix
	Matrix3D( Vector3D Bx, Vector3D By, Vector3D Bz ) ;  // set mtrix columns to these 2 basis vectors

	Vector3D Basis( int column ) ;		// get a basis vector (column)

	void Identity() ;						// set to the identity matrix
	void Scaling( Scalar uniform_scale ) ;	// set to a new scaling matrix
	void Scaling( Vector3D scaling ) ;		// set to a scaling matrix
	void Rotation( double angle, int axis ) ;			// set to a rotation matrix 
	void Shearing( Vector2D shearing, int axis ) ;	// set to a shear matrix 

	void Scale( Scalar uniform_scale ) ;	// multiply by a new scaling matrix
	void Scale( Vector3D scale ) ;			// multiply by a new scaling matrix
	void Rotate( double angle, int axis ) ;			// multiply by a new rotation matrix 
	void Shear( Vector2D shear, int axis ) ;			// multiply by a new shearing matrix

	Matrix3D operator +  ( const Matrix3D& ) const ;	// matrix addition
	Matrix3D operator -  ( const Matrix3D& ) const ;	// matrix subtraction
	Matrix3D operator *  ( const Matrix3D& ) const ;	// matrix multiplication
	void     operator *= ( const Matrix3D& ) ;			// multiply a matrix by another matrix
	Vector3D operator *  ( const Vector3D& ) const ;	// matrix * vector. Used to transform vertices
friend
	void     operator *= ( Vector3D&, const Matrix3D& ) ;// transposed-vector * matrix . Used to transform normals.

	Scalar   Determinant() const ;
	Matrix3D Inverse() ; // returns a new matrix that is the inverse of this matrix
	void     Invert() ; // inverts this matrix in place
	Matrix3D Transpose() const ;
	Scalar   Trace() const ;	// sum of diagonal elements

	// comparisions	
	bool     operator == ( const Matrix3D& ) const ;
	bool     operator != ( const Matrix3D& ) const ;
	static   const Matrix3D& Identity_Matrix() ;	// return an identity matrix to be used in comparisions with other matrices

protected:
	Scalar Determinant2D( int row, int col ) const ;

private:
	static const int dimension = 3 ;
	Scalar m[dimension][dimension] ;
};
