#include "nglBoundingVolume.h"




ngl::physics::BoundingVolume::BoundingVolume()
{
	this->_cachedMeshinfo = new RawMesh();
	this->_cachedMeshinfo->SetDrawType(GL_LINE_STRIP);
	_center = glm::vec3(0,0,0);
}

ngl::physics::BoundingVolume::~BoundingVolume()
{
	delete _cachedMeshinfo;
}
glm::vec3 ngl::physics::BoundingVolume::GetCenter()
{
	return this->_center;
}