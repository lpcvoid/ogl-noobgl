#include "nglAionHeightmap.h"
#include "..\..\rax\raxFileBuffer.h"




bool ngl::loaders::AionHeightmap::LoadFromFile(std::string file)
{
	//aion height structure :
	//array of [(word) height (byte)unknown junk]
	rax::io::FileBuffer buf(file,rax::io::RAX_IO_FILE_MODE_READ);
	int data_count = buf.GetFileSize() / 3;
	_hmap.resize(data_count);

	std::vector<AionHeightEntry>* raw_data;
	raw_data = buf.ReadArray<AionHeightEntry>(data_count);

	for (unsigned int i = 0; i < data_count;i++)
	{
		unsigned short h = (*raw_data)[i].height;
		_hmap[i] = h;
	}

	delete raw_data;

	int sidelen = sqrt((float)data_count);
	_size.x = sidelen;
	_size.y = sidelen;

	return true;
}

float ngl::loaders::AionHeightmap::GetHeight(float x, float y, bool check_params /*= false*/)
{
	
	x /= 2.0f;
	y /= 2.0f;
	int xInt = (int) x;
	int yInt = (int) y;
	unsigned short p1 = GetElement(yInt + xInt * _size.x,check_params);
	unsigned short p2 = GetElement(yInt + 1 + xInt * _size.x,check_params);
	unsigned short p3 = GetElement(yInt + (xInt + 1) * _size.x,check_params);
	unsigned short p4 = GetElement(yInt + 1 + (xInt + 1) * _size.x,check_params);

	float modf_result;
	modf(x,&modf_result);

	float p13 = p1 + (p1 - p3) * modf_result;
	float p24 = p2 + (p4 - p2) * modf_result;

	modf(y,&modf_result);

	float p1234 = p13 + (p24 - p13) * modf_result;
	return p1234 / 32.0;
}

unsigned short ngl::loaders::AionHeightmap::GetElement(int indx, bool safe /*= false*/)
{

	if (safe)
		if (indx >= _hmap.size() || indx < 0)
			return 0;

	return _hmap[indx];
}
