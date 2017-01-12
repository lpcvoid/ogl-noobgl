#include "nglMaterialManager.h"




nglMaterialManager::nglMaterialManager(std::string& dir)
{
	this->_matDefLoader = new nglMaterialDefinitionLoader();
	this->_matDirectory = dir;
}

nglMaterialManager::~nglMaterialManager()
{

}


nglMaterial* nglMaterialManager::GetMaterial(std::string& name)
{



	//load material definition!
	nglMaterial* material = this->GetLoadedMaterial(name);

	if (material != NULL)
	{
		//material already exists.
		return material;
	}	
	else {

		material = this->_matDefLoader->LoadDefinitionFile(this->_matDirectory, name + ".nmd");

		if (material != NULL)
			this->_materials.push_back(material);

		return material;
	}


}

bool nglMaterialManager::HasMaterialLoaded(std::string& name)
{
	return (this->GetLoadedMaterial(name) != NULL);
}

nglMaterial* nglMaterialManager::GetLoadedMaterial(std::string& name)
{
	for (int i = 0; i< this->_materials.size();i++)
	{
		if (this->_materials[i]->GetName() == name)
			return this->_materials[i];
	}

	return NULL;
}
