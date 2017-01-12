#include "nglShaderUniform.h"
#include <string>
#include <iostream>



GLenum nglShaderUniform::GetType()
{
	return this->_type;
}

nglShaderUniform::nglShaderUniform(char* name, GLenum type)
{
	this->SetName(std::string(name));
	this->_type = type;
	this->SetDirty(false); //if we have no data, we don't need to upload uniform again.
}