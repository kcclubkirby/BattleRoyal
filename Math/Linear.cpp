#include "Linear.h"

void Ray2D::Init()
{
	tmin = 0.0 ;
	tmax = Scalar( Math::_HUGE ) ;
}

Ray2D::Ray2D()
{
	Init() ;
}

Ray2D:: Ray2D( Vector2D& origin, Vector2D& direction )
{
	Init() ;
	this->origin = origin ;
	this->direction = direction ;
}

/****      Line     ****/

Line::Line()
{
}

Line::Line( Vector2D point, Vector2D normal )
{
	this->point = point ;
	this->normal = normal ;
	this->normal.Normalize() ;
}

Scalar Line::Distance( Vector2D point )
{
	return normal * ( point - this->point ) ;
}


/****      Intersection     ****/


RayIntersectLine Ray_Intersect_Line( Ray2D ray, Line line )
{
	RayIntersectLine result ;
	double numerator, denominator ;

	numerator = line.normal * line.point - line.normal * ray.origin ;
	denominator = line.normal * ray.direction ;

	if ( Within_Epsilon( denominator ) )
	{
		if ( Within_Epsilon( numerator ) )
		{
			result.type = RayIntersectLine::Coincident ;
			result.t = 0.0 ;
		}
		else
		{
			result.type = RayIntersectLine::Parallel ;
			result.t = Scalar( Math::_HUGE ) ;
		}
	}
	else
	{
		result.t = Scalar( numerator / denominator ) ;
		result.normal = line.normal ;
		result.type = result.t >= 0.0 ? RayIntersectLine::Intersect : RayIntersectLine::NoIntersect ;
		result.point = ray.origin + ray.direction * result.t ;
	}

	return result ;
}


void Ray::Init()
{
	tmin = (Scalar)0.0 ;
	tmax = (Scalar)Math::_HUGE ;
}


const double Plane::epsilon = 1.0e-5 ;

Plane:: Plane( Vector4D& v1, Vector4D& v2, Vector4D& v3 )
{
	Vector4D e1, e2, normal ;
	Scalar distance_to_origin ;
	e1 = v2 - v1 ;
	e2 = v3 - v1 ;
	normal = e1.Cross_Product( e2 ) ;
	normal.Normalize() ;
	distance_to_origin = normal * v1 ;
	p = normal ;
	p[W] = -distance_to_origin ;
}

Vector3D Plane:: Normal()
{
	return Vector3D( p[X], p[Y], p[Z] ) ;
}

double Plane:: Distance( const Vector4D& v ) const
{
	double result ;
	result = p * v ;
	return result ;
}

Plane::Location Plane::Locate( Scalar distance )
{
	if ( distance < -epsilon ) return NegHs ;
	if ( distance >  epsilon ) return PosHs ;
	else return OnHp ;
}

Plane::Location Plane::Locate( Vector4D vector )
{
	Scalar distance = *this * vector ;
	if ( distance < -epsilon ) return NegHs ;
	if ( distance >  epsilon ) return PosHs ;
	else return OnHp ;
}

Scalar Plane:: operator * ( Vector4D& vector )
{
	return p * vector ;
}

Plane  Plane:: operator * ( Matrix4D& matrix )
{
	Plane result ;

	result.p = p * matrix ;

	return result ;
}

Plane Plane::Normalize()
{
	p.Normalize3D() ;
	return *this ;
}


RayIntersection Ray_Intersect_Plane( Ray ray, Plane plane )
{
	RayIntersection result ;
	double demoninator, numerator ;

	numerator = plane * ray.origin ;
	demoninator = -1.0 * ( plane * ray.direction ) ;

	if ( numerator < -Plane::epsilon ) result.location = Plane::NegHs ;
	else if ( numerator >  Plane::epsilon ) result.location = Plane::PosHs ;
	else result.location = Plane::OnHp ;

	if ( Math::fabs( demoninator ) < Plane::epsilon )
		if ( Math::fabs( numerator ) < Plane::epsilon )
		{
			result.kind = RayIntersection::IsOn ;
			result.t = 0.0 ;
		}
		else
		{
			result.kind = RayIntersection::IsParallel ;
			result.t = Math:: _HUGE ;
		}
	else
	{
		result.kind = RayIntersection::Intersects ;
		result.intersects = true ;
		result.t = numerator / demoninator ;
		result.normal = plane.Normal().Normalized() ;
	}
	return result ;
}

Vector3D Reflected_Direction( Vector3D incident, Vector3D normal )
{
	Vector3D reflected ;

	reflected = ( -1.0 * normal * incident ) * normal * 2 + incident ;

	return reflected ;
}
