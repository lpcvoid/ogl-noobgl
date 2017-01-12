#include "nglMesh.h"
#include "gl\glew.h"
#include "nglShaderGlobals.h"
#include "nglTexture.h"
#include "nglOBB.h"
#include "nglBoundingVolume.h"





ngl::scene::Mesh::Mesh()
{
	//set default draw type to triangles
	this->_drawType = GL_TRIANGLES;
	_material = NULL;
	this->ResetTransformation();
	this->_boundingVolume = new ngl::physics::OBB();
}

ngl::scene::Mesh::~Mesh()
{
	delete this->_boundingVolume;
}

bool ngl::scene::Mesh::UpdateBoundingVolume()
{
	return this->_boundingVolume->UpdateBoundingVolume(this->vertices,this->indices);
}

ngl::physics::BoundingVolume* ngl::scene::Mesh::GetBoundingVolume()
{
	return _boundingVolume;
}

void ngl::scene::Mesh::SetMaterial(nglMaterial* material)
{
	_material = material;
}

nglMaterial* ngl::scene::Mesh::GetMaterial()
{
	return _material;
}

bool ngl::scene::Mesh::HasMaterial()
{
	return (this->_material != NULL);
}
