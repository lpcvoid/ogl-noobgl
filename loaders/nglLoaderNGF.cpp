#include "raxFileBuffer.h"
#include "nglLoaderNGF.h"
#include "nglBufferObject.h"
#include "..\nglCommon.h"
#include <string>

nglGeometry* nglLoaderNGF::LoadFromFile(std::string& file)
{
	if (rax::io::FileBuffer::FileExists(file)){

		nglGeometry* geo = new nglGeometry();

		rax::io::FileBuffer raxbuf(file,rax::io::RAX_IO_FILE_MODE_READ);

		int magic = raxbuf.Read<int>();//NGLG
		unsigned char version = raxbuf.Read<unsigned char>();
		
		if (magic != 1196181326 || version != NGLG_VERSION)
			return NULL;

		int mesh_count = raxbuf.Read<int>();

		if (mesh_count > 0)
		{
			for (unsigned int i = 0; i < mesh_count; i++)
			{

				ngl::scene::Mesh* ngf_mesh = new ngl::scene::Mesh();

				//read name
				unsigned char namelen = raxbuf.Read<unsigned char>();
				void* meshname = malloc(namelen + 1);
				ZeroMemory(meshname,namelen + 1);
				raxbuf.ReadData(meshname,namelen);
				ngf_mesh->SetName(std::string((char*)meshname));

				printf((std::string("nglLoaderNGF::LoadFromFile() : Loaded submesh ") + ngf_mesh->GetName() + std::string("\n")).c_str());

				//read position
				nglVec3 mesh_local_pos = raxbuf.Read<nglVec3>();
				ngf_mesh->SetPosition(mesh_local_pos);

				//read scale
				nglVec3 mesh_local_scale = raxbuf.Read<nglVec3>();
				ngf_mesh->SetScale(mesh_local_scale);

				//read transform
				float local_rotation[9]; 
				raxbuf.ReadData(&local_rotation,4*9);

				nglMat3 local_rotation_matrix (
				
					nglVec3(local_rotation[0],local_rotation[1],local_rotation[2]),
					nglVec3(local_rotation[3],local_rotation[4],local_rotation[5]),
					nglVec3(local_rotation[6],local_rotation[7],local_rotation[8])

				);

				ngf_mesh->SetRotation(nglMat4(local_rotation_matrix));

				int vertex_count = raxbuf.Read<int>();
				nglBufferObject<nglVertex3P>* vertex_buf = new nglBufferObject<nglVertex3P>(NGL_ARRAY_DATA_TYPE_VERTEX);
				vertex_buf->PreAllocate(vertex_count);
				for (unsigned int i = 0; i < vertex_count; i++)
				{
					nglVertex3P v = raxbuf.Read<nglVertex3P>();
					vertex_buf->SetElement(v,i);

				}

				int index_count = raxbuf.Read<int>(); //actually face count, so we need to * 3
				nglBufferObject<unsigned short>* index_buf = new nglBufferObject<unsigned short>(NGL_ARRAY_DATA_TYPE_INDEX);
				index_buf->PreAllocate(index_count);
				for (unsigned int i = 0; i < index_count; i++)
				{
					unsigned short indx = raxbuf.Read<unsigned short>();
					index_buf->SetElement(indx,i);

				}

				int normal_count = raxbuf.Read<int>();
				nglBufferObject<nglVertex3P>* normal_buf = new nglBufferObject<nglVertex3P>(NGL_ARRAY_DATA_TYPE_NORMALS);
				normal_buf->PreAllocate(normal_count);
				for (unsigned int i = 0; i < normal_count; i++)
				{
					nglVertex3P v = raxbuf.Read<nglVertex3P>();
					normal_buf->SetElement(v,i);

				}


				int color_count = raxbuf.Read<int>();

				ngf_mesh->SetBufferVertices(vertex_buf);
				ngf_mesh->SetBufferIndices(index_buf);
				ngf_mesh->SetBufferNormals(normal_buf);

				ngf_mesh->UpdateBoundingVolume();

				geo->AddMesh(ngf_mesh);


			}
		}


		return geo;
	} else {

		return NULL;
	}


}

nglLoaderNGF::nglLoaderNGF()
{

}

nglLoaderNGF::~nglLoaderNGF()
{

}

