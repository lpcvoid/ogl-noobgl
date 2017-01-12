#include "nglLoaderCGF.h"
#include "nglGeometry.h"
#include "raxFileBuffer.h"
#include "nglCommon.h"
#include "nglMesh.h"
#include <map>
#include "nglCGFDefs.h"

#define VERTEX_SCALE 0.01f;

using namespace ngl;
using namespace loaders;
using namespace cgf;

void nglLoaderCGF::GetHandledFileType()
{
	throw std::logic_error("The method or operation is not implemented.");
}

nglGeometry* nglLoaderCGF::LoadFromFile(std::string& file)
{
	if (rax::io::FileBuffer::FileExists(file)){

		nglGeometry* geo = new nglGeometry();

		rax::io::FileBuffer raxbuf(file,rax::io::RAX_IO_FILE_MODE_READ);

		char signature[6];

		raxbuf.ReadData(&signature[0],6);

		//TODO : Do something with signature
		raxbuf.SkipBytes(2);
		int cgf_file_type = raxbuf.Read<int>();

		switch(cgf_file_type) {
		case CGF_FILETYPE_ANIMATION:
			break;

		case CGF_FILETYPE_GEOMETRY:
			//read header
			raxbuf.SkipBytes(4); //unknown data
			int cgf_tableoffset = raxbuf.Read<int>();
			raxbuf.Seek(cgf_tableoffset);
			int cgf_chunk_count = raxbuf.Read<int>();
			this->_cgfChunks.resize(cgf_chunk_count);
			for (int i = 0; i < cgf_chunk_count; i++)
			{
				CHUNK_HEADER_0744* chunkheader = this->ReadChunkHeader(raxbuf);
				this->_cgfChunks[i] = chunkheader;
			}

			//process chunks
			for (int i = 0; i < cgf_chunk_count; i++)
			{

				switch(this->_cgfChunks[i]->ChunkType){

					

					case ChunkType_Node:
						raxbuf.Seek(this->_cgfChunks[i]->FileOffset);
						this->LoadNodeChunk(raxbuf);

						break;

					case ChunkType_Mtl:
						raxbuf.Seek(this->_cgfChunks[i]->FileOffset);
						this->LoadMaterialFromChunk(this->_cgfChunks[i]->ChunkID,raxbuf);
						break;
						//mesh chunks are NOT loaded directly!
	// 				case ChunkTypes::ChunkType_Mesh:
	// 					if (this->_cgfChunks[i]->ChunkVersion == 0x744){
	// 						raxbuf.Seek(this->_cgfChunks[i]->FileOffset);
	// 						nglMesh* mesh = this->ReadChunkMesh(raxbuf);
	// 						this->_cgfMeshes.push_back(mesh);
	// 						geo->AddMesh(mesh);
	// 					}
	// 					break;


				}

			}

			break;


		}


		//just for fun
		for (unsigned int i = 0; i < this->_contentcgf->GetNodeCount();i++)
		{
			Mesh* mesh = this->_contentcgf->GetNode(i)->pMesh; 
			if (mesh != NULL)
				//if (mesh->GetVertexCount() > 100)
				geo->AddMesh(mesh);
		}

		return geo;

	} else {

		return NULL;

	}


}

nglLoaderCGF::nglLoaderCGF()
{
	this->_contentcgf = new ngl::loaders::cgf::ContentCGF(std::string("cgf_file"));
}

CHUNK_HEADER_0744* nglLoaderCGF::ReadChunkHeader(rax::io::FileBuffer& raxbuf)
{
	CHUNK_HEADER_0744* chunkheader = new CHUNK_HEADER_0744();
	chunkheader->ChunkType = raxbuf.Read<ChunkTypes>();
	chunkheader->ChunkVersion = raxbuf.Read<int>();
	chunkheader->FileOffset = raxbuf.Read<int>();
	chunkheader->ChunkID = raxbuf.Read<int>();
	return chunkheader;
}

Mesh* nglLoaderCGF::ReadChunkMesh(rax::io::FileBuffer& raxbuf)
{
	raxbuf.SkipBytes(16); //skip chunk header
	raxbuf.SkipBytes(4); //skip some flags
	int verticesCount = raxbuf.Read<int>();
	int uvCount = raxbuf.Read<int>();
	int indexCount = raxbuf.Read<int>();
	raxbuf.SkipBytes(4); //skip vertanim reference, which is always -1 always AFAIK from sources

	

	if (verticesCount > 0 && indexCount > 0){

		Mesh* mesh = new Mesh();

		//https://www.opengl.org/discussion_boards/showthread.php/156620-glVertexPointer-Stride



		std::vector<nglVertex3P> vecvert;
		std::vector<unsigned short> vec_index_vert;

		std::vector<nglVertex3P> vecnormals;
		std::vector<unsigned short> vec_index_normals;

		std::vector<nglVec2> vecuv;
		std::vector<unsigned short> vec_index_uv;
		

		for (int i = 0; i < verticesCount;i++)
		{

			nglVertex3P vert;
			vert.x = raxbuf.Read<float>();
			//direct x has y/z swapped, we fix that here
			vert.z = raxbuf.Read<float>();
			vert.y = raxbuf.Read<float>();

			//we need to downscale this shit now, fuck the aion people
			vert.x /= 100.0;
			vert.y /= 100.0;
			vert.z /= 100.0;
			 
			vecvert.push_back(vert);

			nglVertex3P norm;
			norm.x = raxbuf.Read<float>();
			norm.z = raxbuf.Read<float>();
			norm.y = raxbuf.Read<float>();

			vecnormals.push_back(norm);

		}

		for (int i = 0; i < indexCount;i++)
		{
			//indexCount is actually facecount, which means we need to read 3 indices every time.

			short index;
			index = (unsigned short) raxbuf.Read<int>();
			//in cgf, normals are ver-vertex, and not per-face,
			//so they actually share the same indices.
			vec_index_vert.push_back(index);
			vec_index_normals.push_back(index);

			index = (unsigned short) raxbuf.Read<int>();
			vec_index_vert.push_back(index);
			vec_index_normals.push_back(index);

			index = (unsigned short) raxbuf.Read<int>();
			vec_index_vert.push_back(index);
			vec_index_normals.push_back(index);

			int mat = raxbuf.Read<int>();
			raxbuf.SkipBytes(4); // skip MatID and SmGroup
		}


		if (uvCount > 0){

			//read UV coordinates
			for (int i=0; i<uvCount; i++)
			{
				nglUV uv;
				uv.x = raxbuf.Read<float>();
				uv.y = raxbuf.Read<float>();
				vecuv.push_back(uv);

			}

			//read the index array for uv coordinates
			for (int i = 0; i < indexCount;i++)
			{
				//indexCount is actually facecount, which means we need to read 3 indices every time.
				short index;
				index = (unsigned short) raxbuf.Read<int>();
				vec_index_uv.push_back(index);

				index = (unsigned short) raxbuf.Read<int>();
				vec_index_uv.push_back(index);

				index = (unsigned short) raxbuf.Read<int>();
				vec_index_uv.push_back(index);
			}


		}


		std::vector<nglVertex3P>  sorted_out_vertices; 
		std::vector<nglVec2>  sorted_out_uvs;
		std::vector<nglVertex3P>  sorted_out_normals;

		this->SortBufferData(vecvert,vec_index_vert,vecnormals,vec_index_normals,vecuv,vec_index_uv,sorted_out_vertices,sorted_out_uvs,sorted_out_normals);

		std::vector<unsigned short> final_out_indices; 
		std::vector<nglVertex3P>  final_out_vertices; 
		std::vector<nglVec2>  final_out_uvs;
		std::vector<nglVertex3P>  final_out_normals;

		this->CalculateIndexBuffer(sorted_out_vertices,sorted_out_uvs,sorted_out_normals,final_out_indices,final_out_vertices,final_out_uvs,final_out_normals);

		//now, finally, we can put the data into buffers.

		//vertex
		nglBufferObject<nglVertex3P>* bufvert = new nglBufferObject<nglVertex3P>(NGL_ARRAY_DATA_TYPE_VERTEX);
		bufvert->PreAllocate(verticesCount);
		bufvert->SetBufferData(final_out_vertices,NGL_BUFFEROBJECT_USAGE_STATIC_DRAW);

		//normals
		nglBufferObject<nglVertex3P>* bufnormals = new nglBufferObject<nglVertex3P>(NGL_ARRAY_DATA_TYPE_NORMALS);
		bufnormals->PreAllocate(verticesCount);
		bufnormals->SetBufferData(final_out_normals,NGL_BUFFEROBJECT_USAGE_STATIC_DRAW);

		//indices
		nglBufferObject<unsigned short>* bufindex = new nglBufferObject<unsigned short>(NGL_ARRAY_DATA_TYPE_INDEX);
		bufindex->PreAllocate(indexCount * 3);
		bufindex->SetBufferData(final_out_indices,NGL_BUFFEROBJECT_USAGE_STATIC_DRAW);

		//uvs
		nglBufferObject<nglVec2>* bufuv = new nglBufferObject<nglVec2>(NGL_ARRAY_DATA_TYPE_UV);
		bufuv->PreAllocate(uvCount);
		bufuv->SetBufferData(final_out_uvs, NGL_BUFFEROBJECT_USAGE_STATIC_DRAW);
		
		mesh->SetBufferIndices(bufindex);
		mesh->SetBufferNormals(bufnormals);
		mesh->SetBufferVertices(bufvert);
		mesh->SetBufferUV(bufuv);

		mesh->UpdateBoundingVolume();

		return mesh;

	} else {

		return NULL;

	}

}

void nglLoaderCGF::SortBufferData(
	/*input: seperately indexed data buffers */ 
	std::vector<nglVertex3P>& bufvert, 
	std::vector<unsigned short>& buf_index_vert, 
	std::vector<nglVertex3P>& bufnormals, 
	std::vector<unsigned short>& buf_index_normals, 
	std::vector<nglVec2>& bufuv, 
	std::vector<unsigned short>& buf_index_uv, 
	/*output : unindexed, raw, correctly ordered data */ /*this could already be rendered without an index array! */ 
	std::vector<nglVertex3P> & out_vertices, 
	std::vector<nglVec2> & out_uvs, 
	std::vector<nglVertex3P> & out_normals)
{

	// For each vertex of each triangle
	for( unsigned int i=0; i<buf_index_vert.size(); i++ ){

		// Get the indices of its attributes
		unsigned int vertexIndex = buf_index_vert[i];
		unsigned int uvIndex = buf_index_uv[i];
		unsigned int normalIndex = buf_index_normals[i];

		// Get the attributes thanks to the index
		nglVertex3P vertex = bufvert[ vertexIndex ];
		nglVec2 uv = bufuv[ uvIndex];
		nglVertex3P normal = bufnormals[ normalIndex ];

		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs.push_back(uv);
		out_normals.push_back(normal);

	}

}

void nglLoaderCGF::CalculateIndexBuffer(
	/*input from SortBufferData() */ 
	std::vector<nglVertex3P> & in_vertices, 
	std::vector<nglVec2> & in_uvs, 
	std::vector<nglVertex3P> & in_normals, 
	/*final, ready to render data */ 
	std::vector<unsigned short> & out_indices, 
	std::vector<nglVertex3P> & out_vertices, 
	std::vector<nglVec2> & out_uvs, 
	std::vector<nglVertex3P> & out_normals)
{

	std::map<PackedVertex,unsigned short> VertexToOutIndex;

	// For each input vertex
	for ( unsigned int i=0; i<in_vertices.size(); i++ ){

		PackedVertex packed = {in_vertices[i], in_uvs[i], in_normals[i]};


		// Try to find a similar vertex in out_XXXX
		unsigned short index;
		bool found = this->GetSimilarVertexIndex( packed, VertexToOutIndex, index);

		if ( found ){ // A similar vertex is already in the VBO, use it instead !
			out_indices.push_back( index );
		}else{ // If not, it needs to be added in the output data.
			out_vertices.push_back( in_vertices[i]);
			out_uvs     .push_back( in_uvs[i]);
			out_normals .push_back( in_normals[i]);
			unsigned short newindex = (unsigned short)out_vertices.size() -1; //0 indexed
			out_indices .push_back( newindex );
			VertexToOutIndex[ packed ] = newindex;
		}
	}

}

bool nglLoaderCGF::GetSimilarVertexIndex(PackedVertex & packed, std::map<PackedVertex,unsigned short> & VertexToOutIndex, unsigned short & result)
{
	std::map<PackedVertex,unsigned short>::iterator it = VertexToOutIndex.find(packed);
	if ( it == VertexToOutIndex.end() ){
		return false;
	}else{
		result = it->second;
		return true;
	}
}

//////////////////////////////////////////////////////////////////////////
MaterialCGF* nglLoaderCGF::LoadMaterialFromChunk( int nChunkId , rax::io::FileBuffer& raxbuf)
{
	for (int i = 0; i < _contentcgf->GetMaterialCount(); i++)
	{
		if (_contentcgf->GetMaterial(i)->nChunkId == nChunkId)
			return _contentcgf->GetMaterial(i);
	}

	CHUNK_HEADER_0744* hdr = GetChunkHeader(nChunkId);

// 	if (pChunkDesc->hdr.ChunkType == ChunkType_MtlName)
// 	{
// 		return LoadMaterialNameChunk( pChunkDesc );
// 	}
	if (hdr->ChunkType == ChunkType_Mtl)
	{
		raxbuf.Seek(hdr->FileOffset);
		return LoadOldMaterialChunk( raxbuf );
	}
	return NULL;
}

MaterialCGF* nglLoaderCGF::LoadOldMaterialChunk(rax::io::FileBuffer& raxbuf)
{
	CHUNK_HEADER_0744* hdr = ReadChunkHeader(raxbuf);

	MaterialCGF* pMtlCGF = new MaterialCGF();
	this->_contentcgf->AddMaterial(pMtlCGF);
	pMtlCGF->nChunkId = hdr->ChunkID;

	//read from file
	memset(&pMtlCGF->name[0], 0, 128);
	raxbuf.ReadData(&pMtlCGF->name[0],64);
	raxbuf.SkipBytes(60); //Reserved
	raxbuf.SkipBytes(4); //alphaTest
	raxbuf.SkipBytes(1); //MtlType
	pMtlCGF->col_d = raxbuf.Read<nglRGB>();
	pMtlCGF->col_a = raxbuf.Read<nglRGB>();
	pMtlCGF->col_s = raxbuf.Read<nglRGB>();
	raxbuf.SkipBytes(266); //Strange bytes, which don't correspond to source - but always 266 as far as I can tell.

	//the texture goodie starts
	
	raxbuf.ReadData(&pMtlCGF->texture_diffuse.name[0],128);
	return pMtlCGF;

}

bool nglLoaderCGF::LoadNodeChunk(rax::io::FileBuffer& raxbuf)
{
	NODE_CHUNK_DESC_0823 *nodeChunk = new NODE_CHUNK_DESC_0823();
	nodeChunk->chdr = this->ReadChunkHeader(raxbuf);

	raxbuf.ReadData(&nodeChunk->name[0],64);
	nodeChunk->ObjectID = raxbuf.Read<int>();
	nodeChunk->ParentID = raxbuf.Read<int>();
	nodeChunk->nChildren = raxbuf.Read<int>();
	nodeChunk->MatID = raxbuf.Read<int>();

	nodeChunk->IsGroupHead = raxbuf.Read<bool>();
	nodeChunk->IsGroupMember = raxbuf.Read<bool>();

	raxbuf.SkipBytes(2); //unknown

	raxbuf.ReadData(&nodeChunk->tm,16 * 4);

	nodeChunk->pos = raxbuf.Read<nglVec3>();
	raxbuf.SkipBytes(16); //quaternion xyzw floats
	nodeChunk->scl = raxbuf.Read<nglVec3>();


	nodeChunk->pos_cont_id =  raxbuf.Read<int>();
	nodeChunk->rot_cont_id =  raxbuf.Read<int>();
	nodeChunk->scl_cont_id =  raxbuf.Read<int>();
	nodeChunk->PropStrLen =  raxbuf.Read<int>();

	//construct node
	NodeCGF* pNodeCGF = new NodeCGF();

	// Fill node object.
	pNodeCGF->nChunkId = nodeChunk->chdr->ChunkID;
	pNodeCGF->nParentChunkId = nodeChunk->ParentID;
	pNodeCGF->nObjectChunkId = nodeChunk->ObjectID;
	pNodeCGF->pParent = 0;
	pNodeCGF->pMesh = 0;
	strncpy( pNodeCGF->name,nodeChunk->name,sizeof(pNodeCGF->name) );

	pNodeCGF->pos_cont_id = nodeChunk->pos_cont_id;
	pNodeCGF->rot_cont_id = nodeChunk->rot_cont_id;
	pNodeCGF->scl_cont_id = nodeChunk->scl_cont_id;

	nglVec3 pos = nodeChunk->pos;
	float scale = VERTEX_SCALE;
	pos = (pos * scale);
	pNodeCGF->pos = pos; // Scale 100 times down.

	//pNodeCGF->rot = nodeChunk->rot;

	pNodeCGF->scl = nodeChunk->scl;

	if (pNodeCGF->nParentChunkId > 1)
		pNodeCGF->bIdentityMatrix = false;

	pNodeCGF->pMaterial = 0;
	if (nodeChunk->MatID > 0)
	{
		pNodeCGF->pMaterial = LoadMaterialFromChunk(nodeChunk->MatID,raxbuf);
	}

	glm::mat3 rot (
		glm::vec3(nodeChunk->tm[0][0],nodeChunk->tm[0][1],nodeChunk->tm[0][2]),
		glm::vec3(nodeChunk->tm[1][0],nodeChunk->tm[1][1],nodeChunk->tm[1][2]),
		glm::vec3(nodeChunk->tm[2][0],nodeChunk->tm[2][1],nodeChunk->tm[2][2])
		);

	rot = glm::transpose(rot);

	glm::mat4 rotmat(rot);

// 	pNodeCGF->localTM = nglMat4(
// 		glm::vec4(nodeChunk->tm[0][0],nodeChunk->tm[0][1],nodeChunk->tm[0][2],nodeChunk->tm[0][3]),
// 		glm::vec4(nodeChunk->tm[1][0],nodeChunk->tm[1][1],nodeChunk->tm[1][2],nodeChunk->tm[1][3]),
// 		glm::vec4(nodeChunk->tm[2][0],nodeChunk->tm[2][1],nodeChunk->tm[2][2],nodeChunk->tm[2][3]),
// 		glm::vec4(nodeChunk->tm[3][0] * 0.01f,nodeChunk->tm[3][2]*0.01f,nodeChunk->tm[3][1]*0.01f,nodeChunk->tm[3][3])
// 		);


	//rotmat[3][0] = nodeChunk->tm[3][0] * 0.01f;
	//rotmat[3][1] = nodeChunk->tm[3][2] * 0.01f;
	//rotmat[3][2] = nodeChunk->tm[3][1] * 0.01f;

	pNodeCGF->localTM = rotmat;

	//pNodeCGF->localTM = glm::transpose(pNodeCGF->localTM);

	pNodeCGF->type = NodeCGF::NODE_MESH;

	//now get attached object
	CHUNK_HEADER_0744* hdr_obj = GetChunkHeader(nodeChunk->ObjectID);

	pNodeCGF->pMesh= NULL;
	//It is almost always a mesh...
	if (hdr_obj->ChunkType == ChunkType_Mesh){
		//load the mesh!
		raxbuf.Seek(hdr_obj->FileOffset);
		pNodeCGF->pMesh = this->ReadChunkMesh(raxbuf);
		if (pNodeCGF->pMesh)
			pNodeCGF->pMesh->SetRotation(rotmat);
			pNodeCGF->pMesh->SetPosition(pos);
			pNodeCGF->pMesh->SetScale(pNodeCGF->scl);
	}

	this->_contentcgf->AddNode(pNodeCGF);

	return true;
}

CHUNK_HEADER_0744* nglLoaderCGF::GetChunkHeader(int nChunkId)
{
	for (int i = 0; i < this->_cgfChunks.size(); i++)
	{
		if (this->_cgfChunks[i]->ChunkID == nChunkId)
			return this->_cgfChunks[i];
	}

	return NULL;
}


