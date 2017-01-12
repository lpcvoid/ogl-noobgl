#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <glm/glm.hpp>

#ifndef nglRay_h__
#define nglRay_h__

namespace ngl {

	namespace physics {

		struct Ray {

			glm::vec3 start;
			glm::vec3 direction;
			//length limit. If 0 or negative, unlimited is assumed.
			//not mathematically correct, you dork
			//float limit; 

		};

	}

}

#endif // nglRay_h__