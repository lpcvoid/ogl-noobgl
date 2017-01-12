#include <iostream>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include <vector>

#include "nglMesh.h"
#include "nglSceneNode.h"
#include "nglMaterial.h"
#include "nglRay.h"
#include "nglRenderableContainer.h"

#ifndef nglGeometry_h__
#define nglGeometry_h__

using namespace ngl;
using namespace scene;
using namespace physics;

enum NGL_GEOMETRY_LOD {
	NGL_GEOMETRY_LOD_LOW,
	NGL_GEOMETRY_LOD_MIDDLE,
	NGL_GEOMETRY_LOD_HIGH
};

class nglGeometry : public nglRenderableContainer {

public:
	nglGeometry();
	~nglGeometry();


	nglVec3 GetCenterPosition();


	void AddMesh(ngl::scene::Mesh* mesh);

	bool Intersect(Ray& ray);

	virtual std::vector<ngl::scene::Mesh*> GetMeshes();

	virtual bool HasMesh();

	virtual ngl::scene::Mesh* GetMesh(std::string name);

private:
	std::vector<ngl::scene::Mesh*> _meshes;
};

#endif // nglGeometry_h__