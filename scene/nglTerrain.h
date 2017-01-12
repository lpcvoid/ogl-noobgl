// Include standard headers
#include <stdio.h>
#include <stdlib.h>

#ifndef nglTerrain_h__
#define nglTerrain_h__

#include "nglMesh.h"
#include "nglHeightmap.h"
#include "nglTerrainPatch.h"
#include "nglSceneNode.h"
#include "nglRenderableContainer.h"

class nglTerrain : public nglRenderableContainer {

public:
	nglTerrain();
	virtual ~nglTerrain();
	
	

	bool Create(ngl::loaders::Heightmap* hmap, nglVec2U16 patch_size, int patch_columns, int patch_rows,int data_spacing = 1);


	virtual std::vector<ngl::scene::Mesh*> GetMeshes();

	virtual bool HasMesh();

	virtual ngl::scene::Mesh* GetMesh(std::string name);

private:
	ngl::loaders::Heightmap* _hmap;
	std::vector<nglTerrainPatch*> _patches;
	

};


#endif // nglTerrain_h__