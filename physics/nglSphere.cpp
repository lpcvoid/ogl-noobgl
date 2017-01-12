#include "nglSphere.h"
#include "..\nglCommon.h"




nglVec3 ngl::physics::Sphere::GetPointOnHull(float theta, float phi)
{
	nglVec3 p;
	float theta_rad = NGL_DEGTORAD(theta);
	float phi_rad = NGL_DEGTORAD(phi);

	p.x = position.x + radius * sin(theta_rad) * cos(phi_rad);
	p.y = position.y + radius * cos(theta_rad);
	p.z = position.z + radius * sin(theta_rad) * sin (phi_rad);
	return p;
}

ngl::physics::Sphere::Sphere(nglVec3& pos, float radius)
{
	position = pos;
	this->radius = radius;
}

ngl::physics::Sphere::~Sphere()
{

}
