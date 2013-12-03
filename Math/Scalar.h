#pragma once
#include <assert.h>

namespace Math
{
	#include <math.h>
	const double PI = 3.14159265358979323846 ;
}

typedef float Scalar ;

// these are to make the code more readable. Use vector[X] instead of vector[0], etc. 
const int X = 0 ;
const int Y = 1 ;
const int Z = 2 ;
const int W = 3 ;

inline double Degrees_to_Radians( double degrees )
{
	return degrees / 180.0 * Math::PI ;
}

inline double Radians_to_Degrees( double radians )
{
	return radians * 180.0 / Math::PI ;
}


// Because floating point computation contains numerical error, comparing to 0 is not adequate
// Instead, we need to compare to a number close to 0, which we call "epsilon".
// From experience choosing epsilon somewhere between 1.0e-4 and 1.0e-7 is sufficient, but depends on the details of the computation involved.

const Scalar epsilon = Scalar( 1.0e-5 ) ;

// use this instead of comparing to zero; for example, when avoiding a divide-by-zero
inline bool Within_Epsilon( double value )
{
	return value < epsilon && value > -epsilon ;
}



