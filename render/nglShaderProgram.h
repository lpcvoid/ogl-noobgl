#include <iostream>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "nglShader.h"
#include <vector>
#include "nglShaderUniform.h"
#include <map>
#include "nglObject.h"
#include "nglCommon.h"

#ifndef nglShaderProgram_h__
#define nglShaderProgram_h__



class nglShaderProgram : public nglObject {

public:
	nglShaderProgram();
	~nglShaderProgram();
	void AddShader(nglShader* shader);
	std::vector<nglShader*> GetShaders();
	std::vector<nglShaderUniform*> GetUniforms();

	//these functions are used to describe what inputs the shader needs.
	//renderer uses the get function to decide what to upload to vram.
	//the set function is used by material manager when it loads this.
	//it uses a material definition file to set these flags accordingly.
	void SetInputFlag(NGL_ARRAY_DATA_TYPE flag);
	bool GetInputFlag(NGL_ARRAY_DATA_TYPE flag);

	void AddUniform(nglShaderUniform* uniform);
	void AddUniforms(std::vector<nglShaderUniform*>& uniform_vector);
	bool HasUniform(std::string& name);
	nglShaderUniform* GetUniform(std::string& name);
	
private:
	std::vector<nglShader*> _shaders;
	std::map<std::string,nglShaderUniform*> _uniforms;

	//this describes what inputs the shader expects.
	//see NGL_MATERIAL_INPUT_FLAGS
	//formerly called attributes
	int _inputflags;



};




#endif // nglShaderProgram_h__