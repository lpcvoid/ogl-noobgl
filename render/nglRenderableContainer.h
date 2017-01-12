#include <stdio.h>
#include <stdlib.h>
#include "nglCommon.h"
#include "nglMesh.h"
#include "nglMaterial.h"
#include "nglSceneNode.h"

#ifndef nglRenderableContainer_h__
#define nglRenderableContainer_h__

/*

	A renderable container is a container that contains meshes.
	It can be extended by several classes which maintain collections of meshes.
	It has no own transformation, since it inherits all that from scene node, which in turn gets it by inheriting scene object.

	It can contain a global material which is used if the material associated to meshes is null.
	Finally, the inheriting class must implement a way to obtain meshes!

*/

class nglRenderableContainer : public nglSceneNode {

public:
	nglRenderableContainer();
	virtual ~nglRenderableContainer();

	nglMaterial* GetGlobalMaterial();




	virtual ngl::scene::Mesh* GetMesh(std::string name) = 0;
	virtual std::vector<ngl::scene::Mesh*> GetMeshes() = 0;
	virtual bool HasMesh() = 0;


private:
	//material that is used if a mesh has no material attached
	nglMaterial* _global_material;

};




#endif // nglRenderableContainer_h__