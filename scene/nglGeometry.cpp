#include "nglGeometry.h"
#include "nglSceneNode.h"



nglGeometry::nglGeometry()
{
	this->SetNodeType(NGL_SCENENODE_TYPE_GEOMETRY);
}


bool nglGeometry::Intersect(Ray& ray)
{
	auto meshes = this->GetMeshes();

	for (unsigned int i = 0; i < meshes.size();i++)
	{
		BoundingVolume* bv = meshes[i]->GetBoundingVolume();
		if (bv->Intersect(ray,meshes[i]->GetTransformationMatrix() * this->GetTransformationMatrix()))
			return true;
	}
	return false;
}

std::vector<ngl::scene::Mesh*> nglGeometry::GetMeshes()
{
	return _meshes;
}

bool nglGeometry::HasMesh()
{
	return (_meshes.size() > 0);
}

nglGeometry::~nglGeometry()
{

}

void nglGeometry::AddMesh(ngl::scene::Mesh* mesh)
{
	_meshes.push_back(mesh);
}

ngl::scene::Mesh* nglGeometry::GetMesh(std::string name)
{
	for (unsigned int i = 0; i < _meshes.size();i++)
	{
		if (_meshes[i]->GetName() == name)
			return _meshes[i];
	}

	return NULL;
}

nglVec3 nglGeometry::GetCenterPosition()
{
	nglVec3 median_pos(0,0,0);
	int nMeshes = _meshes.size();
	for (unsigned int i = 0; i < nMeshes;i++)
	{
		median_pos += _meshes[i]->GetPosition();
	}

	median_pos.x /= nMeshes;
	median_pos.y /= nMeshes;
	median_pos.z /= nMeshes;

	return median_pos + GetPosition();
}

