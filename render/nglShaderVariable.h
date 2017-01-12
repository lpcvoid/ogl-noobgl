#include <iostream>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "nglShader.h"
#include <vector>
#include "nglObject.h"

#ifndef nglShaderVariable_h__
#define nglShaderVariable_h__


class nglShaderVariable : public nglObject {

public:
	static const int LOC_UNKNOWN = -2;
	static const int LOC_NOT_DEFINED = -1;

	nglShaderVariable();

	void SetLocation(GLint location);
	GLint GetLocation();

protected:
	// if -2, location not known
	// if -1, not defined in shader
	// if >= 0, uniform defined and available.
	GLint location;
	/**
	 * Name of the variable as was declared in the shader.
	 * E.g name = "g_WorldMatrix" if the decleration was
	 * "uniform mat4 g_WorldMatrix;".
	 */




};

#endif // nglShaderVariable_h__