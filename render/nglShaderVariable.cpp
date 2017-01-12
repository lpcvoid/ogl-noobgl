#include "nglShaderVariable.h"
#include <string>
#include "nglShaderGlobals.h"



GLint nglShaderVariable::GetLocation()
{
	return location;
}

void nglShaderVariable::SetLocation(GLint location)
{
	this->location = location;
	this->SetDirty(true);
}

nglShaderVariable::nglShaderVariable() : location(LOC_UNKNOWN)
{

}
