#include <iostream>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include <vector>
#include "glm/fwd.hpp"
#include "glm/detail/type_vec.hpp"
#include "soil/src/SOIL.h"
#include "nglObject.h"

#ifndef nglTexture_h__
#define nglTexture_h__

class nglTexture : public nglObject{

public:
	nglTexture(GLint minMagFiler = GL_LINEAR, GLint wrapMode = GL_CLAMP_TO_EDGE);
	~nglTexture();

	bool LoadFromFile(std::string file_path);
	bool Load();

	int GetImageWidth();
	int GetImageHeight();

	void SetSize(int width, int height);

	GLint GetMinMagFilter();
	void SetMinMagFilter(GLint val);

	GLint GetWrapMode();
	void SetWrapMode(GLint val);

	void SetTextureCompareFunc(GLint func);
	GLint GetTextureCompareFunc();

	void SetTextureCompareMode(GLint mode);
	GLint GetTextureCompareMode();

	void SetTextureDatatype(GLint dt);
	GLint GetTextureDataType();

	void SetTextureFormat(GLint tf);
	GLint GetTextureFormat();

	void SetTextureInternalFormat(GLint tf);
	GLint GetTextureInternalFormat();


	void FreeImageData();
	bool HasImageData();
	unsigned char* GetImageData();


private:
	int _height,_width;
	GLint _minMagFilter, _wrapMode, _compare_func, _compare_mode,_datatype, _format, _internalformat;
	unsigned char* _image; //texture data
	std::string _imagepath;

};


#endif // nglTexture_h__