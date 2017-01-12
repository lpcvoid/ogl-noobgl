// Include standard headers
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "nglLoader.h"
#include "nglGeometry.h"
#include "raxFileBuffer.h"
#include "nglMesh.h"
#include <map>
#include "ContentCGF.h"
#include "nglCGFDefs.h"

using namespace ngl;
using namespace scene;
using namespace loaders;
using namespace cgf;

#ifndef nglLoaderCGF_h__
#define nglLoaderCGF_h__





class nglLoaderCGF : public nglLoader {

public:

	static const int CGF_FILETYPE_GEOMETRY = 0xFFFF0000;
	static const int CGF_FILETYPE_ANIMATION = 0xFFFF0001;

	nglLoaderCGF();

	virtual void GetHandledFileType();

	
	virtual nglGeometry* LoadFromFile(std::string& file);

private:

	ngl::loaders::cgf::ContentCGF* _contentcgf;

	std::vector<CHUNK_HEADER_0744*> _cgfChunks;
	std::vector<Mesh*> _cgfMeshes;
	CHUNK_HEADER_0744* ReadChunkHeader(rax::io::FileBuffer& raxbuf);
	Mesh* ReadChunkMesh(rax::io::FileBuffer& raxbuf);
	
	bool LoadNodeChunk(rax::io::FileBuffer& raxbuf);
	MaterialCGF* LoadMaterialFromChunk( int nChunkId ,rax::io::FileBuffer& raxbuf);
	CHUNK_HEADER_0744* GetChunkHeader(int nChunkId);
	MaterialCGF* LoadOldMaterialChunk(rax::io::FileBuffer& raxbuf);


	//This combines all seperate data arrays into the same order
	void SortBufferData(		
		
		//input: seperately indexed data buffers

		std::vector<nglVertex3P>& bufvert,
		std::vector<unsigned short>& buf_index_vert,

		std::vector<nglVertex3P>& bufnormals,
		std::vector<unsigned short>& buf_index_normals,

		std::vector<nglVec2>& bufuv,
		std::vector<unsigned short>& buf_index_uv,

		//output : unindexed, raw, correctly ordered data
		//this could already be rendered without an index array!
		std::vector<nglVertex3P> & out_vertices, 
		std::vector<nglVec2> & out_uvs,
		std::vector<nglVertex3P> & out_normals

	);

	struct PackedVertex{
		nglVertex3P position;
		nglVec2 uv;
		nglVertex3P normal;
		bool operator<(const PackedVertex that) const{
			return memcmp((void*)this, (void*)&that, sizeof(PackedVertex))>0;
		};
	};

	//Internal function that checks if we already have data in vbo
	bool GetSimilarVertexIndex( 
		PackedVertex & packed, 
		std::map<PackedVertex,unsigned short> & VertexToOutIndex,
		unsigned short & result
		);


	//this takes the output of SortBufferData(), and creates an index array for those.
	//the index array then contains indices which point to synchronized elements in their respective data arrays.
	//we do this because opengl/d3d only supports one index array for all data arrays, and not multiple index arrays for
	//multiple data arrays.
	void CalculateIndexBuffer(

		//input from SortBufferData()
		std::vector<nglVertex3P> & in_vertices,
		std::vector<nglVec2> & in_uvs,
		std::vector<nglVertex3P> & in_normals,

		//final, ready to render data
		std::vector<unsigned short> & out_indices,
		std::vector<nglVertex3P> & out_vertices,
		std::vector<nglVec2> & out_uvs,
		std::vector<nglVertex3P> & out_normals

	);


};

#endif // nglLoaderCGF_h__