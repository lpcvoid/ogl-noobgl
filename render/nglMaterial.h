// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <glm/glm.hpp>

//include ngl stuff
#include "nglShaderProgram.h"
#include "nglShaderUniform.h"
#include "gtc/type_ptr.hpp"
#include "nglTexture.h"
#include "nglObject.h"
#include "nglCommon.h"
#include <map>

using namespace std;

#ifndef nglMaterial_h__
#define nglMaterial_h__


struct nglMaterialProperties {

	//general
	bool wiremesh;

	//lightning
	nglVec3 color_diffuse;
	nglVec3 color_specular;

	//shadows
	bool cast_shadows;
	bool recieve_shadows;


};


class nglMaterial : public nglObject {

public:
	nglMaterial();
	~nglMaterial();

	//void SetVec3(std::string& attribute_name, glm::vec3& color, bool AddIfNotExistInDef = true);
	//adds a texture to this material.
	//material sets the texture unit for the texture, from material definition file.
	void AddTexture(std::string& attribute_name, nglTexture* tex);

	bool HasTexture();
	std::vector<nglTexture*> GetTextures();

	void SetTextureActive(int index);
	nglTexture* GetActiveTexture();

	//get a parameter for an uniform that was defined in material definition file.
	//void* GetMaterialParameter(std::string& name);

	void SetDiffuseColor(nglVec3& dc);
	nglVec3 GetDiffuseColor();

	//creates a new material instance, copied from this one.
	nglMaterial* Copy();

	void ToggleWiremesh(bool tog);
	bool IsWiremesh();

private:

	nglMaterialProperties _properties;

	//this contains the parameters for the uniforms.
	//std::map<std::string, void*> _uniform_params;

	//used internally to add stuff to the uniform parameters map
	//void SetMaterialParameter(std::string param_name, void* param_data);

	//material can have several textures attached
	std::vector<nglTexture*> _textures;
	nglTexture* _activeTexture;

};

#endif // nglMaterial_h__