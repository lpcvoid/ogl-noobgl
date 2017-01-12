#include <iostream>
#include <GL/glew.h>
#include "glm/glm.hpp"
#ifndef nglShaderBase_h__
#define nglShaderBase_h__

enum NGL_SHADER_TYPE {
	NGL_SHADER_TYPE_VERTEX = GL_VERTEX_SHADER,
	NGL_SHADER_TYPE_FRAGMENT = GL_FRAGMENT_SHADER
};

class nglShader {

public:
	nglShader(NGL_SHADER_TYPE shaderType);
	~nglShader();
	void LoadShaderFromFile(std::string& file_in_shader);
	GLuint GetCompiledShader();
	void SetCompiledShader(GLuint cs);
	bool ShaderCodeAvaliable();
	NGL_SHADER_TYPE GetShaderType();
	std::string GetShaderCode();
	bool IsCompiled();

protected:
	GLuint _compiledShader;
	std::string _shadercode;
	NGL_SHADER_TYPE _type;
	
};

#endif //nglShaderBase_h__