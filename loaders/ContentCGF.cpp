#include "ContentCGF.h"
#include "nglCGFDefs.h"




ngl::loaders::cgf::ContentCGF::ContentCGF(std::string& name)
{
	_filename = name;
}

void ngl::loaders::cgf::ContentCGF::AddNode(NodeCGF* pNode)
{
	_nodes.push_back(pNode);
}



int ngl::loaders::cgf::ContentCGF::GetNodeCount()
{
	return _nodes.size();
}

ngl::loaders::cgf::NodeCGF* ngl::loaders::cgf::ContentCGF::GetNode(int i)
{
	return _nodes[i];
}

void ngl::loaders::cgf::ContentCGF::AddMaterial(MaterialCGF* pNode)
{
	_materials.push_back(pNode);
}

int ngl::loaders::cgf::ContentCGF::GetMaterialCount()
{
	return _materials.size();
}

ngl::loaders::cgf::MaterialCGF* ngl::loaders::cgf::ContentCGF::GetMaterial(int i)
{
	return _materials[i];
}
