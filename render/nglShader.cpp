#include <iostream>
#include <vector>
#include "nglShader.h"
#include "nglFileUtils.h"




GLuint nglShader::GetCompiledShader()
{
	return this->_compiledShader;
}

nglShader::~nglShader()
{

}

nglShader::nglShader(NGL_SHADER_TYPE shaderType) : _compiledShader(INT_MAX)
{
	this->_type = shaderType;
}

void nglShader::LoadShaderFromFile(std::string& file_in_shader)
{
	this->_shadercode = nglFileUtils::ReadStringFile(file_in_shader);
}

NGL_SHADER_TYPE nglShader::GetShaderType()
{
	return this->_type;
}

bool nglShader::ShaderCodeAvaliable()
{
	return (!this->_shadercode.empty());
}

std::string nglShader::GetShaderCode()
{
	return this->_shadercode;
}

void nglShader::SetCompiledShader(GLuint cs)
{
	this->_compiledShader = cs;
}

bool nglShader::IsCompiled()
{
	return (this->_compiledShader != INT_MAX);
}
