// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include "nglHeightmap.h"
#include "nglCommon.h"
#include "nglMesh.h"

#ifndef nglTerrainPatch_h__
#define nglTerrainPatch_h__

class nglTerrainPatch {

public:
	nglTerrainPatch(ngl::loaders::Heightmap* hmap, int x, int y, nglVec2U16 size,int data_spacing = 1);
	~nglTerrainPatch();

	ngl::scene::Mesh* GetPatchMesh();

private:
	ngl::scene::Mesh* _mesh;

	int _x;
	int _y;
	nglVec2U16 _size;

	nglBufferObject<nglVertex3P>* GenerateVertices(ngl::loaders::Heightmap* hmap, nglVec2U16 patch_size, int data_spacing = 1);
	nglBufferObject<nglVertex3P>* GenerateNormals(nglBufferObject<nglVertex3P>* vertices, nglBufferObject<unsigned short>* indices);

	nglBufferObject<nglVec2>* GenerateUVCoords(nglVec2U16 patch_size, int data_spacing);
	



	nglBufferObject<unsigned short>* GenerateIndices(nglVec2U16 patch_size);
	nglBufferObject<nglRGB3F>* GenerateColors(ngl::loaders::Heightmap* hmap, nglVec2U16 patch_size);

};

#endif // nglTerrainPatch_h__