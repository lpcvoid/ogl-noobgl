#include <GL/glew.h>
#include "glm/glm.hpp"
#include "nglBufferObject.h"
#include "../scene/nglRawMesh.h"
#include "nglRay.h"

#ifndef nglBoundingVolume_h__
#define nglBoundingVolume_h__

namespace ngl {

	using namespace scene;

	namespace physics {


		struct BVCollisionResult {

			bool collision;

		};


		class BoundingVolume : public nglObject{

		public:
			BoundingVolume();
			virtual ~BoundingVolume();

			virtual bool UpdateBoundingVolume(nglBufferObject<nglVertex3P>* vertices, nglBufferObject<unsigned short>* indices) = 0;

			//Returns a bool depending on collision result.
			//If "result" param isn't NULL, then it will also contain more infos.
			virtual bool Collide(BoundingVolume* otherbv, glm::mat4& worldmat, BVCollisionResult* result) = 0;
			virtual bool Intersect (Ray& ray, glm::mat4& world_transform) = 0;

			virtual RawMesh* GetRenderVolume () = 0; 

			glm::vec3 GetCenter();

		protected:
			//centerpoint in model space
			//can be approximated
			glm::vec3 _center;

			//stuff for rendering the BV
			RawMesh* _cachedMeshinfo;

			virtual void CalculateRenderVolume() = 0;
			
		};

	}

}



#endif // nglBoundingVolume_h__