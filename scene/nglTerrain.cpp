#include "nglTerrain.h"
#include "nglTerrainPatch.h"





nglTerrain::nglTerrain()
{
	this->SetNodeType(NGL_SCENENODE_TYPE_TERRAIN);
	_hmap = NULL;
}

nglTerrain::~nglTerrain()
{

}

bool nglTerrain::Create(ngl::loaders::Heightmap* hmap, nglVec2U16 patch_size, int patch_columns, int patch_rows, int data_spacing)
{
	_hmap = hmap;
	for (unsigned int rows = 0; rows < patch_rows; rows++)
	{

		for (unsigned int cols = 0; cols < patch_columns; cols++){

			nglTerrainPatch* patch = new nglTerrainPatch(hmap,cols, rows, patch_size);
			_patches.push_back(patch);

		}

	}
	return true;
}



std::vector<ngl::scene::Mesh*> nglTerrain::GetMeshes()
{
	std::vector<ngl::scene::Mesh*> meshes;
	for (unsigned int i = 0; i < _patches.size();i++)
	{
		meshes.push_back(_patches[i]->GetPatchMesh());
	}
	return meshes;
}

bool nglTerrain::HasMesh()
{
	return (_patches.size() > 0);
}

ngl::scene::Mesh* nglTerrain::GetMesh(std::string name)
{
	for (unsigned int i = 0; i < _patches.size();i++)
	{
		if (_patches[i]->GetPatchMesh()->GetName() == name)
			return _patches[i]->GetPatchMesh();
	}

	return NULL;
}
