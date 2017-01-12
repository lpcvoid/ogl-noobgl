#include "nglShaderProgram.h"
#include "nglShader.h"
#include "nglFileUtils.h"
#include <vector>
#include "nglShaderUniform.h"

nglShaderProgram::nglShaderProgram()
{
	_inputflags = 0;
}

nglShaderProgram::~nglShaderProgram()
{

}

void nglShaderProgram::SetInputFlag(NGL_ARRAY_DATA_TYPE flag)
{
	this->_inputflags |= flag;
}

bool nglShaderProgram::GetInputFlag(NGL_ARRAY_DATA_TYPE flag)
{
	return (this->_inputflags & flag) > 0;
}

void nglShaderProgram::AddShader(nglShader* shader)
{
	this->_shaders.push_back(shader);
}

std::vector<nglShader*> nglShaderProgram::GetShaders()
{
	return this->_shaders;
}

void nglShaderProgram::AddUniform(nglShaderUniform* uniform)
{
	if (!this->HasUniform(uniform->GetName()))
		this->_uniforms[uniform->GetName()] = uniform;
}

std::vector<nglShaderUniform*> nglShaderProgram::GetUniforms()
{

	std::vector<nglShaderUniform*> ua;

	for( auto it = _uniforms.begin(); it != _uniforms.end(); ++it ) {
		ua.push_back( it->second );
	}

	return ua;
}

void nglShaderProgram::AddUniforms(std::vector<nglShaderUniform*>& uniform_vector)
{

	for (int i = 0; i < uniform_vector.size(); i++)
	{
		this->AddUniform(uniform_vector[i]);
	}
}

bool nglShaderProgram::HasUniform(std::string& name)
{
	return (this->_uniforms.count(name) > 0);
}

nglShaderUniform* nglShaderProgram::GetUniform(std::string& name)
{
	
	nglShaderUniform* uniform = NULL;

	auto it_uniform = this->_uniforms.find(name);

	if (it_uniform != this->_uniforms.end()){

		//first = key
		//second = value
		uniform = it_uniform->second;

	} 

	return uniform;
}

