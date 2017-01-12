#include "nglRenderableContainer.h"




nglRenderableContainer::nglRenderableContainer()
{
	this->SetNodeType(NGL_SCENENODE_TYPE_RENDERABLECONTAINER);
	_global_material = new nglMaterial();

}

nglRenderableContainer::~nglRenderableContainer()
{

}

nglMaterial* nglRenderableContainer::GetGlobalMaterial()
{
	return _global_material;
}