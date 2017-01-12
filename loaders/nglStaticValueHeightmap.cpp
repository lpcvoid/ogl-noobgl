#include "nglStaticValueHeightmap.h"
#include "raxBits.h"




bool ngl::loaders::StaticValueHeightmap::LoadFromFile(std::string file)
{
	return true;
}

float ngl::loaders::StaticValueHeightmap::GetHeight(float x, float y, bool check_params /*= false*/)
{
	return _static_value;
}

nglRGB3F ngl::loaders::StaticValueHeightmap::GetColorValue(int x, int y, bool check_params /*= false*/)
{
	return nglVec3(1,1,1);
}

ngl::loaders::StaticValueHeightmap::StaticValueHeightmap()
{
	_static_value = 0.0f;
	rax::utils::Bits::SetFlag<int>(_provided_data,ngl::loaders::HEIGHTMAP_DATA_HEIGHT);
}

ngl::loaders::StaticValueHeightmap::~StaticValueHeightmap()
{

}

void ngl::loaders::StaticValueHeightmap::SetStaticValue(float val)
{
	_static_value = val;
}

float ngl::loaders::StaticValueHeightmap::GetStaticValue()
{
	return _static_value;
}
