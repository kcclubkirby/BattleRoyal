#pragma once

#include "Vector2D.h"

class Matrix2D
{
public:
	Matrix2D(void) ;						// set to the zero matrix
	Matrix2D( Vector2D Bx, Vector2D By ) ;  // set mtrix columns to these 2 basis vectors

	void Identity() ;						// set to the identity matrix
	void Scaling( Vector2D scaling ) ;		// set to a scaling matrix
	void Rotation( double angle ) ;			// set to a rotation matrix 
	void Shearing( Vector2D shearing ) ;	// set to a shear matrix 

	void Scale( Scalar uniform_scale ) ;	// multiply by a new scaling matrix
	void Scale( Vector2D scale ) ;			// multiply by a new scaling matrix
	void Rotate( double angle ) ;			// multiply by a new rotation matrix 
	void Shear( Vector2D shear ) ;			// multiply by a new shearing matrix

	Matrix2D operator +  ( const Matrix2D& ) const ;	// matrix addition
	Matrix2D operator -  ( const Matrix2D& ) const ;	// matrix subtraction
	Matrix2D operator *  ( const Matrix2D& ) const ;	// matrix multiplication
	void     operator *= ( const Matrix2D& ) ;			// multiply a matrix by another matrix
	Vector2D operator *  ( const Vector2D& ) const ;	// matrix * vector. Used to transform vertices
friend
	void     operator *= ( Vector2D&, const Matrix2D& ) ;// transposed-vector * matrix . Used to transform normals.

	Scalar   Determinant() const ;
	Matrix2D Inverse() ; // returns a new matrix that is the inverse of this matrix
	void     Invert() ; // inverts this matrix in place

	// comparisions	
	bool     operator == ( const Matrix2D& ) const ;
	bool     operator != ( const Matrix2D& ) const ;
	static   const Matrix2D& Identity_Matrix() ;	// return an identity matrix to be used in comparisions with other matrices

private:
	static const int dimension = 2 ;
	Scalar m[dimension][dimension] ;
};
