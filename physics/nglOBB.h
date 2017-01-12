#include "nglBoundingVolume.h"
#include "nglRawMesh.h"


#ifndef nglOBB_h__
#define nglOBB_h__

namespace ngl {

	using namespace scene;

	namespace physics {

		

		class OBB : public BoundingVolume {

		public:

			OBB();

			//update the internal mesh
			bool UpdateBoundingVolume(nglBufferObject<nglVertex3P>* vertices, nglBufferObject<unsigned short>* indices);

			virtual bool Collide(BoundingVolume* otherbv, glm::mat4& worldmat, BVCollisionResult* result);
			bool Intersect (Ray& ray, glm::mat4& world_transform);

			virtual RawMesh* GetRenderVolume();


			glm::vec3 GetPositiveHalfWidth();
			glm::vec3 GetNegativeHalfWidth();

			//bool Collide(BoundingVolume* otherbv, glm::mat4& worldmat, BVCollisionResult* result)

		private:
			glm::vec3 _posHalfwidth; //halfwidth of positive side
			glm::vec3 _negHalfwidth; //halfwidth of negative side

			virtual void CalculateRenderVolume();
		};

	}

}

#endif // nglOBB_h__