#pragma once

#include "Vector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"

class Matrix4D
{
public:
	Matrix4D(void) ;						// set to the zero matrix
	Matrix4D( Vector3D Bx, Vector3D By, Vector3D Bz ) ;  // set matrix columns to these 3 basis vectors; 4th column will be [0 0 0 1]
	Matrix4D( Vector4D Bx, Vector4D By, Vector4D Bz, Vector4D Bw ) ;  // set matrix columns to these basis vectors

	void Identity() ;								// set to the identity matrix
	void Scaling( Scalar uniform_scale ) ;			// set to a scaling matrix
	void Scaling( Vector3D scaling ) ;				// set to a scaling matrix
	void Rotation( double angle, int axis ) ;		// set to a rotation matrix 
	void Shearing( Vector2D shearing, int axis ) ;	// set to a shear matrix 
	void Translation( Vector3D& translation ) ;		// set to a 3D translation matrix 

	void Scale( Scalar uniform_scale ) ;		// multiply by a new scaling matrix
	void Scale( Vector3D scale ) ;				// multiply by a new scaling matrix
	void Rotate( double angle, int axis ) ;		// multiply by a new rotation matrix 
	void Shear( Vector2D shear, int axis ) ;	// multiply by a new shearing matrix
	void Translate( Vector3D& translation ) ;	// multiply by a new 3D translation matrix 
	void Translate( Vector4D& translation ) ;	// multiply by a new 3D translation matrix 

	Matrix4D operator +  ( const Matrix4D& ) const ;	// matrix addition
	Matrix4D operator -  ( const Matrix4D& ) const ;	// matrix subtraction
	void     operator += ( const Matrix4D& )       ;	// matrix addition
	void     operator -= ( const Matrix4D& )       ;	// matrix subtraction
	Matrix4D operator *  ( const Matrix4D& ) const ;	// matrix multiplication
	void     operator *= ( const Matrix4D& ) ;			// multiply a matrix by another matrix

	Vector4D operator *  ( const Vector4D& ) const ;	// matrix * vector. Used to transform vertices
friend
	Vector4D operator * ( Vector4D&, Matrix4D& ) ;		 // vector * matrix ; e.g transforming a plane
friend
	void     operator *= ( Vector4D&, const Matrix4D& ) ;// transposed-vector * matrix . Used to transform normals.

	Scalar   Determinant() const ;
	Matrix4D Inverse() const ; // returns a new matrix that is the inverse of this matrix
	void     Invert() ; // inverts this matrix in place
	Matrix4D Transpose() const ;
	Scalar   Trace() const ;	// sum of diagonal elements
	
	bool     operator == ( const Matrix4D& ) const ;
	bool     operator != ( const Matrix4D& ) const ;
	static   const Matrix4D& Identity_Matrix() ;	// return an identity matrix to be used in comparisions with other matrices

protected:
	Scalar Determinant2D( int row, int col ) const ;
	Scalar Determinant3D( int row, int col ) const ;

private:
	static const int dimension = 4 ;
	Scalar m[dimension][dimension] ;
};
