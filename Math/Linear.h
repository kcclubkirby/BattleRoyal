#pragma once
#include "Vector2D.h"
#include "Vector4D.h"
#include "Matrix4D.h"

class Ray2D ;
struct RayIntersectLine ;

class Line
{
private:
	Vector2D point ;
	Vector2D normal ;

public:
	Line() ;
	Line( Vector2D point, Vector2D normal ) ;

	Scalar Distance( Vector2D point ) ;

	friend RayIntersectLine Ray_Intersect_Line( Ray2D ray, Line line ) ;
};

class Ray2D
{
public:
	Vector2D origin ;
	Vector2D direction ;
	Scalar   tmin, tmax ;

	void Init() ;

public:
	Ray2D() ;
	Ray2D( Vector2D& origin, Vector2D& direction ) ;

	friend RayIntersectLine Ray_Intersect_Line( Ray2D ray, Line line ) ;
} ;

struct RayIntersectLine
{
	Scalar t ;
	Vector2D point ;
	Vector2D normal ;
	enum { NoIntersect, Intersect, Parallel, Coincident } type ;

	RayIntersectLine()
	{
		t = Scalar( 0 ) ;
	}
} ;

// 3D Version

struct RayIntersection ; // forward declaration

class Ray
{
private:
	void Init() ;

public:
	Vector4D origin, direction ;
	double tmin, tmax ;

	Ray(void)
	{
		Init() ;
	}

	Ray( Vector4D origin, Vector4D direction )
	{
		this->origin = origin ;
		this->direction = direction ;
		Init() ;
	}

	Ray( Vector3D origin, Vector3D direction )
	{
		this->origin = Vector4D( origin, 1 ) ;
		this->direction = Vector4D( direction, 0 ) ;
		Init() ;
	}

	Vector4D Point( double t )
	{
		return origin + direction * (Scalar)t ;
	}

	friend RayIntersection Ray_Intersect_Plane( Ray, class Plane ) ;

} ;

class Plane
{

public:
	static const double epsilon ;
	Vector4D p ;

public:
	Plane() {}
	Plane( Vector4D& v1, Vector4D& v2, Vector4D& v3 ) ;

	Scalar   operator *  ( Vector4D& ) ;	// dot product between a point and a plane
	Plane    operator *  ( Matrix4D& ) ;	// transforming a plane

	Vector3D Normal() ;
	Plane    Normalize() ;
	double   Distance( const Vector4D& pt ) const ;
	enum     Location { NegHs, PosHs, BothHs, OnHp } ;
	Location Locate( Vector4D ) ;
	static   Location Locate( Scalar ) ;

	friend RayIntersection Ray_Intersect_Plane( Ray, Plane ) ;
} ;

struct RayIntersection
{
	Plane::Location location ;  // of the ray.orgin wrt the plane
	enum { Intersects, IsParallel, IsOn } kind ;
	Vector3D normal ;
	double t, length ;
	bool intersects ;

	RayIntersection()
	{
		location = Plane::BothHs ;
		kind = IsOn ;
		t = length = 0.0 ;
		intersects = false ;
	}
} ;



