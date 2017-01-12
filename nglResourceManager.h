#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <glm/glm.hpp>
#include "nglCommon.h"
#include "nglMesh.h"
#include <map>

#ifndef nglResourceManager_h__
#define nglResourceManager_h__



namespace ngl {

	using namespace scene;

	class ResourceManager {

	public:
		ResourceManager(char* dir_shaders, char* dir_models, char* dir_textures, char* dir_heightmaps, char* dir_guis);
		virtual ~ResourceManager();

		nglShaderProgram* GetShaderProgram(char* name);
		nglTexture* GetTexture(char* name);


	private:
		
		//root directory of executable, with full path
		std::string _dir_root;

		//relative root directory for various data
		std::string _dir_shaders;
		std::string _dir_models;
		std::string _dir_textures;
		std::string _dir_heightmaps;
		std::string _dir_guis;

		std::string _directory_seperator;

		std::vector<Mesh*> _meshes;


		std::map<std::string, nglShaderProgram*> _shaders;
		std::map<std::string, nglTexture*> _textures;


		//gets root path under windows.
		//example : "C:/application"
		std::string GetRootPath();

		//nglShaderBuilder _shader_builder;

		//cached resources
		

	};



}

#endif // nglResourceManager_h__