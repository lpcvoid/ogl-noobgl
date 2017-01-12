#include "nglMaterialDefinitionLoader.h"
#include "tinyxml/tinyxml.h"
#include <glm/glm.hpp>
#include <string>
#include <iostream>
#include "nglShaderProgram.h"
#include "nglShader.h"
#include "nglCommon.h"
#include "raxStringUtils.h"

nglMaterialDefinitionLoader::nglMaterialDefinitionLoader()
{

}

nglMaterial* nglMaterialDefinitionLoader::LoadDefinitionFile(std::string& dir, std::string& file)
{

	nglMaterial* material = new nglMaterial();
	material->SetName(file);
	nglShaderProgram* shaderprog = material->GetShaderProgram();

	std::string dir_file = dir +"/"+ file;

	TiXmlDocument nmdFile(dir_file.c_str());
	nmdFile.LoadFile();

	TiXmlElement* level_material = nmdFile.FirstChildElement("material");
	TiXmlElement* level_uniforms;
	TiXmlElement* level_inputs;
	TiXmlElement* level_shaders;
	TiXmlElement* xml_uniform;
	TiXmlElement* xml_shader;
	TiXmlElement* xml_input;
	if (level_material != NULL){

		//read all inputs!
		//these correspond to the shaders in parameters.

		level_inputs = level_material->FirstChildElement("inputs");
		if (level_inputs != NULL){

			xml_input = level_inputs->FirstChildElement("input");
			while (xml_input)
			{
				std::string str_location(xml_input->Attribute("location"));
				std::string str_type(xml_input->Attribute("type"));

				int location = rax::utils::StringUtils::StringToNumber<int>(str_location);

				if (str_type == "vertices")
					material->SetInputFlag(NGL_ARRAY_DATA_TYPE_VERTEX);

				if (str_type == "uvcoords")
					material->SetInputFlag(NGL_ARRAY_DATA_TYPE_UV);

				if (str_type == "normals")
					material->SetInputFlag(NGL_ARRAY_DATA_TYPE_NORMALS);
				
				if (str_type == "colors")
					material->SetInputFlag(NGL_ARRAY_DATA_TYPE_COLORS);

				xml_input = xml_input->NextSiblingElement("input");

			}


		} else {
			return false;
			//There's no inputs tag.

		}


		//read all uniforms!
		level_uniforms = level_material->FirstChildElement("uniforms");
		if (level_uniforms != NULL){

			xml_uniform = level_uniforms->FirstChildElement("uniform");
			while (xml_uniform)
			{
				std::string uniform_name(xml_uniform->GetText());
				std::string uniform_type_string(xml_uniform->Attribute("datatype"));
				const char* usage_attribute = xml_uniform->Attribute("usage");

				std::string uniform_usage_string;

				if (usage_attribute != NULL)
					uniform_usage_string = usage_attribute;
				else
					uniform_usage_string = "";

				NGL_SHADER_UNIFORM_TYPE uniform_type = nglShaderUniform::GetUniformTypeString(uniform_type_string);
				NGL_SHADER_UNIFORM_USAGE usage_type = nglShaderUniform::GetUniformUsageString(uniform_usage_string);
				nglShaderUniform* uniform = new nglShaderUniform(uniform_name, uniform_type,usage_type);

				//load additional data/parameters for the uniforms.
				//for example, textures get to save the texture unit.
// 				switch(uniform_type) {
// 
// 					case NGL_SHADER_UNIFORM_TYPE_TEXTURE2D:
// 						nglShaderUniformAdditionalInfo_Texture* add_texture = new nglShaderUniformAdditionalInfo_Texture();
// 						std::string uniform_texunit(xml_uniform->Attribute("texunit"));
// 						GLint texunit = nglUtils::StringToNumber<GLint>(uniform_texunit);
// 						if (texunit >= 0 && texunit <= 15)
// 							add_texture->texture_unit = texunit;
// 						else{
// 							printf("nglMaterialDefinitionLoader::LoadDefinitionFile() : Error loading texture unit from definition!");
// 						}
// 						uniform->SetAdditionalData(add_texture);
// 						break;
// 
// 				}

				shaderprog->AddUniform(uniform);

				xml_uniform = xml_uniform->NextSiblingElement("uniform");

			}


		} else {
			return false;
			//There's no uniform tag.

		}

		//read all shader files!
		level_shaders = level_material->FirstChildElement("shaders");
		if (level_shaders != NULL){

			xml_shader = level_shaders->FirstChildElement("shader");
			while (xml_shader)
			{
				const char* shader_type = xml_shader->Attribute("type");

				if (strcmp(shader_type,"vertex") == 0){

					this->_vertexShader = std::string(xml_shader->GetText());

					nglShader* vertex_shader = new nglShader(NGL_SHADER_TYPE_VERTEX);
					vertex_shader->LoadShaderFromFile(dir + "/" + this->GetVertexShaderName());
					shaderprog->AddShader(vertex_shader);

				}else
				if (strcmp(shader_type,"fragment") == 0)
				{
					this->_fragmentShader = std::string(xml_shader->GetText());
					nglShader* fragment_shader = new nglShader(NGL_SHADER_TYPE_FRAGMENT);
					fragment_shader->LoadShaderFromFile(dir + "/" + this->GetFragmentShaderName());
					shaderprog->AddShader(fragment_shader);
				}
				xml_shader = xml_shader->NextSiblingElement("shader");

			}

		} else {
			return NULL;
			//There's no shaders tag.

		}


		//file loaded successfully
		return material;


	} else {
		return NULL;
		//root tag isn't named "material", or doesn't exist.
	}

}

std::string nglMaterialDefinitionLoader::GetVertexShaderName()
{
	return this->_vertexShader;
}

std::string nglMaterialDefinitionLoader::GetFragmentShaderName()
{
	return this->_fragmentShader;
}




