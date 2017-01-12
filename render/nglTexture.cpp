#include "nglTexture.h"




nglTexture::nglTexture(GLint minMagFiler, GLint wrapMode)
{
	this->_minMagFilter = minMagFiler;
	this->_wrapMode = wrapMode;
	this->_compare_func = GL_LEQUAL;
	this->_compare_mode = GL_NONE;
	this->_format = GL_RGB;
	this->_datatype = GL_UNSIGNED_BYTE;
	this->_internalformat = GL_RGB;
	this->_image = NULL;
	this->_imagepath.clear();
}

nglTexture::~nglTexture()
{

}

bool nglTexture::LoadFromFile(std::string file_path)
{
	this->_imagepath = file_path;
	return this->Load();
}


bool nglTexture::Load()
{
	if (!this->HasImageData() && !_imagepath.empty())
	{
		this->_image = SOIL_load_image(this->_imagepath.c_str(), &_width, &_height, 0, SOIL_LOAD_RGB);
		this->SetDirty(true);
	}
	
	return this->HasImageData();

}

void nglTexture::FreeImageData()
{
	if (this->HasImageData()){

		SOIL_free_image_data(this->_image);
	}
}

unsigned char* nglTexture::GetImageData()
{
	return this->_image;
}

bool nglTexture::HasImageData()
{
	return this->_image != NULL;
}

void nglTexture::SetWrapMode(GLint val)
{
	_wrapMode = val;
}

GLint nglTexture::GetWrapMode()
{
	return _wrapMode;
}

void nglTexture::SetMinMagFilter(GLint val)
{
	_minMagFilter = val;
}

GLint nglTexture::GetMinMagFilter()
{
	return _minMagFilter;
}

int nglTexture::GetImageHeight()
{
	return this->_height;
}

int nglTexture::GetImageWidth()
{
	return this->_width;
}

void nglTexture::SetTextureCompareFunc(GLint func)
{
	_compare_func = func;
}

GLint nglTexture::GetTextureCompareFunc()
{
	return _compare_func;
}

void nglTexture::SetTextureCompareMode(GLint mode)
{
	_compare_mode = mode;
}

GLint nglTexture::GetTextureCompareMode()
{
	return _compare_mode;
}

void nglTexture::SetTextureDatatype(GLint dt)
{
	_datatype = dt;
}

GLint nglTexture::GetTextureDataType()
{
	return _datatype;
}

void nglTexture::SetTextureFormat(GLint tf)
{
	_format = tf;
}

GLint nglTexture::GetTextureFormat()
{
	return _format;
}

void nglTexture::SetTextureInternalFormat(GLint tf)
{
	_internalformat = tf;
}

GLint nglTexture::GetTextureInternalFormat()
{
	return _internalformat;
}

void nglTexture::SetSize(int width, int height)
{
	_width = width;
	_height = height;
}



