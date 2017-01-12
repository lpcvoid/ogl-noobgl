#include <GL/glew.h>
#include "glm/glm.hpp"
#include <vector>
#include "nglShaderVariable.h"
#include <string>
#include <iostream>

#ifndef nglShaderUniform_h__
#define nglShaderUniform_h__

// 
// enum NGL_SHADER_UNIFORM_TYPE {
// 
// 	NGL_SHADER_UNIFORM_TYPE_VEC2,
// 	NGL_SHADER_UNIFORM_TYPE_VEC3,
// 	NGL_SHADER_UNIFORM_TYPE_VEC4,
// 	NGL_SHADER_UNIFORM_TYPE_MAT4,
// 	NGL_SHADER_UNIFORM_TYPE_INT,
// 	NGL_SHADER_UNIFORM_TYPE_FLOAT,
// 	NGL_SHADER_UNIFORM_TYPE_BOOL,
// 	NGL_SHADER_UNIFORM_TYPE_TEXTURE2D
// 
// };
// 
// enum NGL_SHADER_UNIFORM_USAGE {
// 
// 	NGL_SHADER_UNIFORM_USAGE_UNDEFINED, //uniform is undefined, and is fed data from material
// 	NGL_SHADER_UNIFORM_USAGE_CAMERA_VP, //uniform is meant to hold the camera view projection matrix
// 	NGL_SHADER_UNIFORM_USAGE_MODEL_TRANSFORMATION //uniform is meant to hold the geometry transformation matrix
// };

class nglShaderUniform : public nglShaderVariable{
public :
	nglShaderUniform(char* name, GLenum type);
	GLenum GetType();

protected:
	GLenum _type;
};




#endif // nglShaderUniform_h__