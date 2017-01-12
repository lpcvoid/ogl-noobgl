#include <iostream>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include <vector>
#include <map>
#include "nglMaterial.h"
#include "nglMaterialDefinitionLoader.h"

#ifndef nglMaterialManager_h__
#define nglMaterialManager_h__


class nglMaterialManager {

public:
	nglMaterialManager(std::string& dir);
	~nglMaterialManager();

	nglMaterial* GetMaterial(std::string& name);
	

private:
	std::vector<nglMaterial*> _materials; 
	bool HasMaterialLoaded(std::string& name);
	nglMaterial* GetLoadedMaterial(std::string& name);

	nglMaterialDefinitionLoader* _matDefLoader;
	std::string _matDirectory;
};

#endif // nglMaterialManager_h__