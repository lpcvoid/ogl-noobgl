#include "nglHeightmap.h"




nglVec2 ngl::loaders::Heightmap::GetSize()
{
	return _size;
}

ngl::loaders::Heightmap::~Heightmap()
{

}

int ngl::loaders::Heightmap::GetProvidedDataMask()
{
	return _provided_data;
}

ngl::loaders::Heightmap::Heightmap()
{
	_provided_data = 0;
}
