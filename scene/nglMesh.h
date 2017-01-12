// Include standard headers
#include <stdio.h>
#include <stdlib.h>

#include "nglBufferObject.h"
#include "nglObject.h"
#include <vector>
#include "nglCommon.h"
#include "nglBoundingVolume.h"
#include <nglRawMesh.h>
#include <nglMaterial.h>
#include "nglSceneObject.h"

#ifndef nglMesh_h__
#define nglMesh_h__

namespace ngl {

	namespace scene {

		class Mesh : public RawMesh, public ngl::scene::SceneObject {

		public:

			Mesh();
			virtual ~Mesh();

			bool UpdateBoundingVolume();
			ngl::physics::BoundingVolume* GetBoundingVolume();

			void SetMaterial(nglMaterial* material);
			nglMaterial* GetMaterial();
			bool HasMaterial();
			
		private:



			ngl::physics::BoundingVolume* _boundingVolume; 
			nglMaterial* _material;
		};

	}

}


#endif // nglMesh_h__