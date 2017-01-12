#include <iostream>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include <vector>
#include "glm/fwd.hpp"
#include "glm/detail/type_vec.hpp"
#include "nglScene.h"
#include "nglSceneNode.h"
#include "nglGeometry.h"
#include "nglCamera.h"
#include "nglShaderProgram.h"
#include "nglTexture.h"
#include <map>
#include <nglMesh.h>
#include <nglRawMesh.h>

#ifndef nglRenderQueue_h__
#define nglRenderQueue_h__

namespace ngl {


	namespace render {

		//represents one mesh, along with the mesh's world transform (gotten from nglGeometry)
		struct RenderQueueEntryMesh {

			RawMesh* mesh;
			//transform of mesh within geometry
			nglMat4 local_transform;
			//transform of geometry within world
			nglMat4 world_transform;

		};

		struct RenderQueueList {
			std::vector<RenderQueueEntryMesh> meshes;
			~RenderQueueList();
			//only dumbly copies BV meshes.
			void AddBoundingVolumeMeshesFromContainer(nglRenderableContainer* cont);

		};


		//contains lists of material:mesh relationships
		class RenderQueue {

		public:
			RenderQueue();
			~RenderQueue();

			int GetListCount();
			//This retrieves a list
			RenderQueueList* GetList(int i);


			//this method intelligently handles the rendering of a container.
			void AddContainer(nglRenderableContainer* container);

			//this method manually adds a list to renderqueue
			void AddList(RenderQueueList* list);

			void Clear();

		private:
			std::vector<RenderQueueList*> _lists;

			//find a list that uses a certain material 
			RenderQueueList* FindList(nglMaterial* mat);


		};

	}


}

#endif // nglRenderQueue_h__