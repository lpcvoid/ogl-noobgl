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

	_mesh->SetDrawType(GL_TRIANGLE_STRIP);
	//_mesh->SetDrawType(GL_LINES);

	if (rax::utils::Bits::FlagSet<int>(hmap->GetProvidedDataMask(), ngl::loaders::HEIGHTMAP_DATA_HEIGHT) )
	{
		//calculate global displacement of heightmap



		auto vertex_buffer = GenerateVertices(hmap,size,nglVec2U16(x,y),data_spacing);
		_mesh->SetBufferVertices(vertex_buffer);


		//now calculate index data
		auto indices = GenerateIndices(size);
		_mesh->SetBufferIndices(indices);



	}

	if (rax::utils::Bits::FlagSet<int>(hmap->GetProvidedDataMask(), ngl::loaders::HEIGHTMAP_DATA_COLORS) )
	{
		auto colors = GenerateColors(hmap,size,nglVec2U16(x,y));
		_mesh->SetBufferColors(colors);
	}

	_mesh->UpdateBoundingVolume();


}

nglTerrainPatch::~nglTerrainPatch()
{

}

ngl::scene::Mesh* nglTerrainPatch::GetPatchMesh()
{
	return _mesh;
}

nglBufferObject<nglVertex3P>* nglTerrainPatch::GenerateVertices(ngl::loaders::Heightmap* hmap,nglVec2U16 patch_size, nglVec2U16 heightmap_col_row, int data_spacing)
{
	nglBufferObject<nglVertex3P>* vertex_buffer = new nglBufferObject<nglVertex3P>(NGL_ARRAY_DATA_TYPE_VERTEX);

	//calculate total number of vertices
	vertex_buffer->PreAllocate((patch_size.x) * (patch_size.y));

	int patch_offset_x = heightmap_col_row.x * patch_size.x;
	int patch_offset_y = heightmap_col_row.y * patch_size.y;

	int index = 0;
	for ( int z = 0; z < patch_size.y; z += data_spacing )
	{

		nglVertex3P vertex;
		vertex.z = z + patch_offset_y - heightmap_col_row.y;

		for ( int x = 0; x < patch_size.x; x += data_spacing )
		{


			
			vertex.x = x + patch_offset_x - heightmap_col_row.x;

			vertex.y = hmap->GetHeight(vertex.x ,vertex.z );
			
			vertex_buffer->SetElement(vertex,index++);
		}
	}
	return vertex_buffer;
}

nglBufferObject<nglRGB3F>* nglTerrainPatch::GenerateColors(ngl::loaders::Heightmap* hmap, nglVec2U16 patch_size, nglVec2U16 heightmap_col_row)
{
	nglBufferObject<nglRGB3F>* color_buffer = new nglBufferObject<nglRGB3F>(NGL_ARRAY_DATA_TYPE_COLORS);

	//calculate total number of vertices
	color_buffer->PreAllocate((patch_size.x) * (patch_size.y));

	int patch_offset_x = heightmap_col_row.x * patch_size.x;
	int patch_offset_y = heightmap_col_row.y * patch_size.y;

	int index =0;

	for ( int z = 0; z < patch_size.y; z++ )
	{
		
		for ( int x = 0; x < patch_size.x; x++ )
		{

			nglRGB3F rgb;
			rgb = hmap->GetColorValue(x + patch_offset_x - heightmap_col_row.x ,z + patch_offset_y - heightmap_col_row.y );

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


