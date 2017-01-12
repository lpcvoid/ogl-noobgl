#include "nglTerrainPatch.h"
#include "raxBits.h"
#include "nglHeightmap.h"
#include "..\nglBufferObject.h"





nglTerrainPatch::nglTerrainPatch(ngl::loaders::Heightmap* hmap, int x, int y, nglVec2U16 size, int data_spacing)
{
	_x = x;
	_y = y;
	_size = size;

	_mesh = new ngl::scene::Mesh();

	_mesh->Move(nglVec3(_x * size.x,0, _y * size.y));
	//_mesh->SetUniformScale(3.0);

	_mesh->SetDrawType(GL_TRIANGLE_STRIP);
	//_mesh->SetDrawType(GL_LINES);

	if (rax::utils::Bits::FlagSet<int>(hmap->GetProvidedDataMask(), ngl::loaders::HEIGHTMAP_DATA_HEIGHT) )
	{
		auto vertex_buffer = GenerateVertices(hmap,size,data_spacing);
		_mesh->SetBufferVertices(vertex_buffer);


		//now calculate index data
		auto indices = GenerateIndices(size);
		_mesh->SetBufferIndices(indices);


		auto normals = GenerateNormals(vertex_buffer,indices);
		_mesh->SetBufferNormals(normals);

		auto uv = GenerateUVCoords(size,data_spacing);
		_mesh->SetBufferUV(uv);

	}

	if (rax::utils::Bits::FlagSet<int>(hmap->GetProvidedDataMask(), ngl::loaders::HEIGHTMAP_DATA_COLORS) )
	{
		auto colors = GenerateColors(hmap,size);
		_mesh->SetBufferColors(colors);
	}

	_mesh->UpdateBoundingVolume();


}

nglTerrainPatch::~nglTerrainPatch()
{
	delete _mesh;
}

ngl::scene::Mesh* nglTerrainPatch::GetPatchMesh()
{
	return _mesh;
}

nglBufferObject<nglVertex3P>* nglTerrainPatch::GenerateVertices(ngl::loaders::Heightmap* hmap,nglVec2U16 patch_size, int data_spacing)
{
	nglBufferObject<nglVertex3P>* vertex_buffer = new nglBufferObject<nglVertex3P>(NGL_ARRAY_DATA_TYPE_VERTEX);

	//calculate total number of vertices
	vertex_buffer->PreAllocate((patch_size.x) * (patch_size.y));



	int index = 0;
	for ( int z = 0; z < patch_size.y; z += data_spacing )
	{

		nglVertex3P vertex;
		vertex.z = z - _y;

		for ( int x = 0; x < patch_size.x; x += data_spacing )
		{
			
			vertex.x = x - _x;

			vertex.y = hmap->GetHeight(vertex.x + (patch_size.x * _x) ,vertex.z  + (patch_size.y * _y));

			
			vertex_buffer->SetElement(vertex,index++);
		}
	}
	return vertex_buffer;
}

nglBufferObject<nglVertex3P>* nglTerrainPatch::GenerateNormals(nglBufferObject<nglVertex3P>* vertices, nglBufferObject<unsigned short>* indices)
{
	nglBufferObject<nglVertex3P>* normal_buffer = new nglBufferObject<nglVertex3P>(NGL_ARRAY_DATA_TYPE_NORMALS);

	//calculate total number of normals
	normal_buffer->PreAllocate(vertices->CountElements());

	int element_count = indices->CountElements();

	for ( unsigned int i = 0; i < element_count -3; i += 3 )
	{
		glm::vec3 v0 = vertices->GetElement( indices->GetElement(i));
		glm::vec3 v1 = vertices->GetElement( indices->GetElement(i+1));
		glm::vec3 v2 = vertices->GetElement( indices->GetElement(i+2));

		glm::vec3 normal = glm::normalize( glm::cross( v1 - v0, v2 - v0 ) );

		if (normal.y < 0.0)
			normal.y *= (-1.0);

		normal_buffer->SetElement(normal, indices->GetElement(i));
		normal_buffer->SetElement(normal, indices->GetElement(i+1));
		normal_buffer->SetElement(normal, indices->GetElement(i+2));
	}



 	return normal_buffer;
}

nglBufferObject<nglRGB3F>* nglTerrainPatch::GenerateColors(ngl::loaders::Heightmap* hmap, nglVec2U16 patch_size)
{
	nglBufferObject<nglRGB3F>* color_buffer = new nglBufferObject<nglRGB3F>(NGL_ARRAY_DATA_TYPE_COLORS);

	//calculate total number of vertices
	color_buffer->PreAllocate((patch_size.x) * (patch_size.y));



	int index =0;

	for ( int z = 0; z < patch_size.y; z++ )
	{
		
		for ( int x = 0; x < patch_size.x; x++ )
		{

			nglRGB3F rgb;
			rgb = hmap->GetColorValue(x + (patch_size.x * _x) - _x ,z  + (patch_size.y * _y) - _y);

			color_buffer->SetElement(rgb,index++);
		}
	}
	return color_buffer;
}

nglBufferObject<unsigned short>* nglTerrainPatch::GenerateIndices(nglVec2U16 patch_size)
{

	int numIndices = (patch_size.x * 2) * (patch_size.y - 1) + (patch_size.y - 2);

	nglBufferObject<unsigned short>* index_buffer = new nglBufferObject<unsigned short>(NGL_ARRAY_DATA_TYPE_INDEX);
	index_buffer->PreAllocate(numIndices);

	int index = 0;
	for ( int z = 0; z < patch_size.y - 1; z++ )
	{
		// Even rows move left to right, odd rows move right to left.
		if ( z % 2 == 0 )
		{
			// Even row
			int x;
			for ( x = 0; x < patch_size.x; x++ )
			{
				index_buffer->SetElement(x + (z * patch_size.x),index++);
				index_buffer->SetElement(x + (z * patch_size.x) + patch_size.x,index++);
			}
			// Insert degenerate vertex if this isn't the last row
			if ( z != patch_size.y - 2)
			{
				index_buffer->SetElement( --x + (z * patch_size.x), index++);
			}
		}
		else
		{
			// Odd row
			int x;
			for ( x = patch_size.x - 1; x >= 0; x-- )
			{
				index_buffer->SetElement(x + (z * patch_size.x),index++);
				index_buffer->SetElement(x + (z * patch_size.x) + patch_size.x,index++);
			}
			// Insert degenerate vertex if this isn't the last row
			if ( z != patch_size.y - 2)
			{
				index_buffer->SetElement(++x + (z * patch_size.x),index++);
			}
		}
	} 

	return index_buffer;
}

nglBufferObject<nglVec2>* nglTerrainPatch::GenerateUVCoords(nglVec2U16 patch_size, int data_spacing)
{
	//CIwFVec2 *uv = pUV + index; 
	//uv->x = v->x / (width-1); 
	//uv->y = v->y / (height-1);
	nglBufferObject<nglVec2>* uv_buffer = new nglBufferObject<nglVec2>(NGL_ARRAY_DATA_TYPE_UV);

	uv_buffer->PreAllocate((patch_size.x) * (patch_size.y));

	int index = 0;
	for ( int z = 0; z < patch_size.y; z += data_spacing )
	{

		nglVec2 uv;

		for ( int x = 0; x < patch_size.x; x += data_spacing )
		{

			uv.x = x / (patch_size.x -1);
			uv.y = z / (patch_size.y -1);
			uv_buffer->SetElement(uv,index++);
		}
	}
	return uv_buffer;
}




