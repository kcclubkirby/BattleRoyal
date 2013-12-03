
#include "Physics.h"

const float gravitational_accleration = -9.8f ;

Physics::Physics(void)
{
	mass = 0.0 ;
	inverse_mass = 0.0 ;
	coefficient.friction = 0.0 ;
	coefficient.restitution = 1.0 ;
	coefficient.drag = Scalar( 1 ) ;
}

Physics::~Physics(void)
{
}

void Physics::Set_Mass( double mass )
{
	this->mass = (Scalar)mass ;
	gravity[Y] = gravitational_accleration * this->mass ;
	inverse_mass = mass != Math::_HUGE ?  Scalar( 1.0 / mass ) : 0 ;
}

Scalar Physics::Speed()
{
	return velocity.Length() ;
}

Vector3D Physics::dX( double dt )
{
	Scalar delta_t = (Scalar) dt ;
	acceleration =  force / mass ;
    
	Vector3D initial_velocity = velocity ;
	Vector3D final_velocity = ( acceleration ) * delta_t + initial_velocity ;
	Vector3D average_velocity = (final_velocity + initial_velocity ) * 0.5f * coefficient.drag ;
	Vector3D dX = average_velocity * delta_t ;
    
	velocity = final_velocity;
	momentum = mass * velocity ;
	position += dX ;
	return dX ;
}

Vector3D Physics::dW( Scalar dt )
{
	Scalar delta_t = (Scalar) dt;
	
	Vector3D initial_angular_v = angular_velocity;
	Vector3D final_angular_v = angular_acceleration * delta_t + initial_angular_v ;
	Vector3D average_angular_v = (final_angular_v + initial_angular_v) * 0.5f;
	Vector3D dW = average_angular_v * delta_t;

	angular_velocity = final_angular_v;
	angular_momentum = inertia_tensor.matrix * angular_velocity;
	return dW;
}

							/****  Contact  ****/

Contact::Contact()
{
	COR = 1.0;
	collided = false;
	penetration = -Scalar(Math::HUGE);
}

void Contact::Compute_Response()
{
	if (collided == false)
		return;

    // get simpler names to use in equations
    Vector3D v1 = this->object[0]->physics.velocity ;
    Vector3D v2 = this->object[1]->physics.velocity ;
    Scalar m1_inverse = this->object[0]->physics.inverse_mass ;
    Scalar m2_inverse = this->object[1]->physics.inverse_mass ;
	Vector3D w1 = this->object[0]->physics.angular_velocity ;
    Vector3D w2 = this->object[1]->physics.angular_velocity ;

    Matrix3D I1_inv = this->object[0]->rotation *
        this->object[0]->inertia_tensor.inverse * this->object[0]->rotation.Transpose();
    Matrix3D I2_inv = this->object[1]->rotation *
        this->object[1]->inertia_tensor.inverse * this->object[1]->rotation.Transpose();
    Vector3D r1 = point - ( this->object[0]->translation + this->object[0]->center_of_mass );
    Vector3D r2 = point - ( this->object[1]->translation + this->object[1]->center_of_mass );
    Vector3D w1_i = I1_inv * ( r1 % normal ) ;
    Vector3D w2_i = I2_inv * ( r2 % normal ) ;

    Vector3D relative_velocityA = ( v1 + w1 % r1 );
	Vector3D relative_velocityB = ( v2 + w2 % r2 );
	Vector3D relative_velocity = relative_velocityA - relative_velocityB;
    Scalar relative_velocity_N = relative_velocity * (normal) ;

    // compute impulse vector
    Scalar e = - ( 1 + COR ) ;
    Scalar dV = e * relative_velocity_N ;
    // change in velocity per unit impulse
    Scalar dV1_i = m1_inverse + ( w1_i % r1 ) * normal ;
    Scalar dV2_i = m2_inverse + ( w2_i % r2 ) * normal ;

    Scalar i = relative_velocity_N / ( m1_inverse + m2_inverse ) ;
	Vector3D impulse = i * (-normal) ; //This needed to be negative, because the normal always points the wrong direction
	impulse *= COR;
	
    // update momenta
	this->object[0]->physics.momentum += impulse * 2;
	this->object[1]->physics.momentum -= impulse * 2;	//This needed to be changed to double, because otherwise it just
														//changes the momentum to zero every time
	Vector3D impulseR = r1 % impulse;
    this->object[0]->physics.angular_momentum += (r1 % impulse) ;
    this->object[1]->physics.angular_momentum -= (r2 % impulse) ;

    this->object[0]->physics.angular_velocity = I1_inv * this->object[0]->physics.angular_momentum ;
    this->object[1]->physics.angular_velocity = I2_inv * this->object[1]->physics.angular_momentum ;
	if (this->object[0]->physics.mass == Math::HUGE)
		this->object[0]->physics.angular_velocity = Vector3D::Zero_Vector();
	if (this->object[1]->physics.mass == Math::HUGE)
		this->object[1]->physics.angular_velocity = Vector3D::Zero_Vector();

	//This next line adds the coefficient of restitution into the equation
	//It was not asked for in the iLab instructions, but I was able to get it to work
	//Sometimes it will glitch, because the physics is based off delta t
	//this->object[0]->physics.momentum = this->object[0]->physics.momentum * this->COR;
	//this->object[1]->physics.momentum = this->object[1]->physics.momentum * this->COR;

    PhysicsObject* object[2] ; // two object that are possibly colliding
    // update velocities
	this->object[0]->physics.velocity = this->object[0]->physics.momentum * m1_inverse ;
    this->object[1]->physics.velocity = this->object[1]->physics.momentum * m2_inverse ;
}


							/****  PhysicsObject  ****/

PhysicsObject:: PhysicsObject()
{
	in_motion = false ;
	up_to_date = false ;
	scaling.Identity() ;
	rotation.Identity() ;
}

Matrix4D& PhysicsObject::Matrix()
{
	if ( !up_to_date )
	{
		Matrix3D composite = rotation * scaling ;
		matrix = Matrix4D( composite.Basis( X ), composite.Basis( Y ), composite.Basis( Z )  ) ;
		matrix.Translate( translation ) ;
		inverse = matrix.Inverse() ;
		up_to_date = true ;
	}

	return matrix ;
}

Matrix4D& PhysicsObject:: Inverse()
{
	Matrix() ;
	return inverse ;
}

Matrix3D& PhysicsObject:: Scaling()
{
	return scaling ;
}

void PhysicsObject:: Scale3D( Vector3D scale )
{
	scaling.Scale( scale ) ;

	up_to_date = false ;
}

void PhysicsObject:: Rotate3D( double angle, int axis )
{
	Matrix3D dR ;
	dR.Rotation( angle, axis ) ;
	rotation *= dR ;

	up_to_date = false ;
}

void PhysicsObject:: Translate3D( Vector3D T )
{
	translation += T ;

	up_to_date = false ;
}

void PhysicsObject:: Update( float delta_t )
{
	Vector3D T = physics.dX( delta_t ) ;
	Translate3D( T ) ;

	in_motion = T.Length() > epsilon ;
}

Vector3D PhysicsObject:: Reflected_Direction( Vector3D incident, Vector3D normal )
{
	Vector3D reflected ;

	reflected = ( -1.0 * normal * incident ) * normal * 2 + incident ;

	return reflected ;
}


//InertiaTensor

InertiaTensor:: InertiaTensor()
{
	matrix.Identity() ;
    inverse.Identity() ;
}

InertiaTensor:: InertiaTensor( double Ixx, double Iyy, double Izz )
{
	matrix.Scaling(
    Vector3D( Scalar( Ixx ), Scalar( Iyy ), Scalar( Izz ) ) ) ;
    inverse = matrix.Inverse() ;
}

void InertiaTensor:: Scale( const Matrix3D& scaling )
{

    Matrix3D scaling_sq = scaling * scaling ;
    matrix.Scale( scaling_sq.Trace() ) ;
    matrix = matrix - scaling_sq ;
    inverse = matrix.Inverse() ;

} 