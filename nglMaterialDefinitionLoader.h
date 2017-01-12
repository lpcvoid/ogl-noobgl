// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <iostream>

#ifndef nglMaterialDefinitionLoader_h__
#define nglMaterialDefinitionLoader_h__

//tinyxml
#include "tinyxml/tinyxml.h"

#include "nglShaderUniform.h"
#include "nglMaterial.h"


class nglMaterialDefinitionLoader{

public:
	nglMaterialDefinitionLoader();
	nglMaterial* LoadDefinitionFile(std::string& dir, std::string& file);

	std::string GetVertexShaderName();
	std::string GetFragmentShaderName();

private:
	std::string _vertexShader, _fragmentShader;

	NGL_SHADER_UNIFORM_TYPE GetUniformType(std::string& type_name);
	


	
};

#endif // nglMaterialDefinitionLoader_h__