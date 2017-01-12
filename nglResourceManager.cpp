#include "nglResourceManager.h"
#include <scene\nglMesh.h>
#include "nglShaderGlobals.h"
#include "raxFileBuffer.h"
#include <windows.h>
#include <shlwapi.h>
#include "nglFileUtils.h"

using namespace ngl;
using namespace scene;


ngl::ResourceManager::ResourceManager(char* dir_shaders, char* dir_models, char* dir_textures, char* dir_heightmaps, char* dir_guis)
{
	_dir_root = this->GetRootPath();
	_dir_shaders = std::string(dir_shaders);
	_dir_models = std::string(dir_models);
	_dir_textures = std::string(dir_textures);
	_dir_heightmaps = std::string(dir_heightmaps);
	_dir_guis = std::string(dir_guis);

	_directory_seperator = NGL_RESOURCEMANAGER_DIR_SEPERATOR;

	//_shader_builder = nglShaderBuilder( _dir_root + _directory_seperator + _dir_shaders + _directory_seperator);
}

ngl::ResourceManager::~ResourceManager()
{

}

std::string ngl::ResourceManager::GetRootPath()
{
	HMODULE hModule = GetModuleHandleW(NULL);
	char path[MAX_PATH];
	GetModuleFileNameA(hModule, path, MAX_PATH);
	PathRemoveFileSpecA(&path[0]);
	return std::string(path);
}


nglShaderProgram* ngl::ResourceManager::GetShaderProgram(char* name)
{

	nglShaderProgram* shdr;
	std::string shdr_name(name);

	auto it = this->_shaders.find(shdr_name);

	if (it != this->_shaders.end()){

		//first = key
		//second = value
		shdr = it->second;

	} else {

		shdr = new nglShaderProgram();

		std::string shdr_path = _dir_root + _directory_seperator + _dir_shaders + _directory_seperator;
		std::string filepath_vs = shdr_path + shdr_name + NGL_SHADER_FILENAME_SUFFIX_VERTEX;
		std::string filepath_fs = shdr_path + shdr_name + NGL_SHADER_FILENAME_SUFFIX_FRAGMENT;


		if (rax::io::FileBuffer::FileExists(filepath_vs)){

			nglShader* vertex_shader = new nglShader(NGL_SHADER_TYPE_VERTEX);
			vertex_shader->LoadShaderFromFile(filepath_vs);
			shdr->AddShader(vertex_shader);
		}

		if (rax::io::FileBuffer::FileExists(filepath_fs)){

			nglShader* fragment_shader = new nglShader(NGL_SHADER_TYPE_FRAGMENT);
			fragment_shader->LoadShaderFromFile(filepath_fs);
			shdr->AddShader(fragment_shader);
		}


		_shaders[shdr_name] = shdr;

	}


	return shdr;

}

nglTexture* ngl::ResourceManager::GetTexture(char* name)
{
	nglTexture* tex;
	std::string str_texture(name);

	auto it = this->_textures.find(str_texture);

	if (it != this->_textures.end()){

		//first = key
		//second = value
		tex = it->second;

	} else {

		tex = new nglTexture();

		std::string path = _dir_root + _directory_seperator + _dir_textures + _directory_seperator;

		tex->LoadFromFile(path + name);

		_textures[str_texture] = tex;

	}

	return tex;

}
