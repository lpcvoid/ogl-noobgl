#include <stdio.h>
#include "..\nglCommon.h"

#ifndef nglSphere_h__
#define nglSphere_h__

namespace ngl {

	namespace physics {

		struct Sphere {


			Sphere(nglVec3& pos, float radius);
			virtual ~Sphere();

			nglVec3 position;
			float radius;


			//obtains point on hull that is defined by theta and phi
			//theta = "vertical", polar angle, orthogonal to phi plane
			//phi = "horizontal", orthogonal to the zenith, azimuth angle
			nglVec3 GetPointOnHull(float theta, float phi);

		};

	}

}

#endif // nglSphere_h__