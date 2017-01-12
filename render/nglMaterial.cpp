#include "nglMaterial.h"
#include "nglShaderUniform.h"
#include <glm/glm.hpp>
#include "nglShader.h"
#include "nglShaderProgram.h"
#include <iostream>
#include <string>
#include "nglShaderGlobals.h"
#include <glm/gtc/type_ptr.hpp>
#include "nglTexture.h"
#include "nglCommon.h"


nglMaterial::nglMaterial()
{


	this->_properties.color_diffuse = nglVec3(1,1,1); //white
	this->_properties.color_specular = nglVec3(1,1,1); //white
	this->_properties.wiremesh = false;
	//set default active texture to NULL
	this->_activeTexture = NULL;
	//we don't want wiremesh active by default.
}

nglMaterial::~nglMaterial()
{

}

// void nglMaterial::SetVec3(std::string& attribute_name, glm::vec3& color,bool AddIfNotExistInDef)
// {
// 	glm::vec3* pvec3 = new glm::vec3(color);
// 
// 	void* data = glm::value_ptr((*pvec3));
// 
// 	this->SetMaterialParameter(attribute_name,data);
// }

void nglMaterial::AddTexture(std::string& attribute_name, nglTexture* tex)
{


	this->_textures.push_back(tex);

	//just as dummy
	//this->SetMaterialParameter(attribute_name,this);

	//set texture as active texture, but only if we have no current active!
	if (this->_activeTexture == NULL)
		this->_activeTexture = tex;

}

bool nglMaterial::HasTexture()
{
	return this->_textures.size() > 0;
}

std::vector<nglTexture*> nglMaterial::GetTextures()
{
	return this->_textures;
}

void nglMaterial::SetTextureActive(int index)
{
	if (this->_textures.size() > index && index >= 0){
		this->_activeTexture = this->_textures[index];
		this->SetDirty(true);
	}
		
}

nglTexture* nglMaterial::GetActiveTexture()
{
	return this->_activeTexture;
}



// void nglMaterial::SetMaterialParameter(std::string param_name, void* param_data)
// {
// 	this->_uniform_params[param_name] = param_data;
// }

// void* nglMaterial::GetMaterialParameter(std::string& name)
// {
// 	auto it = _uniform_params.find(name);
// 
// 	if (it != _uniform_params.end())
// 		return it->second;
// 	else
// 		return NULL;
// }

nglMaterial* nglMaterial::Copy()
{
	nglMaterial* newmat = new nglMaterial();
	newmat->_activeTexture = _activeTexture;
	//newmat->_uniform_params = _uniform_params;
	newmat->_textures = _textures;
	newmat->_name = _name + "_copy";
	return newmat;
}

void nglMaterial::SetDiffuseColor(nglVec3& dc)
{
	this->_properties.color_diffuse = dc;
}

nglVec3 nglMaterial::GetDiffuseColor()
{
	return this->_properties.color_diffuse;
}

void nglMaterial::ToggleWiremesh(bool tog)
{
	this->_properties.wiremesh = tog;
}

bool nglMaterial::IsWiremesh()
{
	return this->_properties.wiremesh;
}




