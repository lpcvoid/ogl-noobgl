#include "nglShaderCompiler.h"
#include "nglShaderProgram.h"
#include "nglShaderGlobals.h"
#include <GL/glew.h>





nglShaderCompiler::nglShaderCompiler(rax::logging::Logging* logger)
{
	this->_logger = logger;
}

nglShaderCompiler::nglShaderCompiler()
{
	this->_logger = NULL;
}

bool nglShaderCompiler::CompileShaderProgram(nglShaderProgram* shaderprog, GLuint* out_shader_id)
{

	GLint shaderprogramID = glCreateProgram();
	*out_shader_id = shaderprogramID;

	std::vector<nglShader*> shaders = shaderprog->GetShaders();

	for (int i = 0; i < shaders.size();i++)
	{
		GLuint shader_temp_id;
		if (this->CompileShader(shaders[i],&shader_temp_id))
			glAttachShader(shaderprogramID,shader_temp_id);
		else
			printf("nglRenderer::CompileShaderProgram() : Didn't attach shader because it failed!");
	}

	//now, we link the shaders.
	glLinkProgram(shaderprogramID);

	// Check Program
	GLint Result = GL_FALSE;
	glGetProgramiv(shaderprogramID, GL_LINK_STATUS, &Result);
	if ( !Result ){

		*out_shader_id = NGL_SHADER_ERROR;

		int InfoLogLength;
		glGetProgramiv(shaderprogramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(shaderprogramID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("nglRenderer::CompileShaderProgram() : %s\n", &VertexShaderErrorMessage[0]);
		return false;
	}


	//add uniforms and attributes to shader

	//set attribute flags of shader
	//check all possible attributes
	GLint attribute_id = -1;
	
	//vertex
	attribute_id = glGetAttribLocation(shaderprogramID,NGL_SHADER_IN_VERTEX_VERTEXDATA);
	if (attribute_id != -1)
		shaderprog->SetInputFlag(NGL_ARRAY_DATA_TYPE_VERTEX);

	//uv
	attribute_id = glGetAttribLocation(shaderprogramID,NGL_SHADER_IN_VERTEX_UVCOORDS);
	if (attribute_id != -1)
		shaderprog->SetInputFlag(NGL_ARRAY_DATA_TYPE_UV);

	//normals
	attribute_id = glGetAttribLocation(shaderprogramID,NGL_SHADER_IN_VERTEX_NORMALS);
	if (attribute_id != -1)
		shaderprog->SetInputFlag(NGL_ARRAY_DATA_TYPE_NORMALS);

	//colors
	//attribute_id = glGetAttribLocation​(shaderprogramID,NGL_SHADER_IN_VERTEX_COLORS);
	attribute_id = glGetAttribLocation(shaderprogramID,NGL_SHADER_IN_VERTEX_COLORS);
	if (attribute_id != -1)
		shaderprog->SetInputFlag(NGL_ARRAY_DATA_TYPE_COLORS);


	//now add all uniforms
	int uniform_count = -1;
	glGetProgramiv( shaderprogramID, GL_ACTIVE_UNIFORMS, &uniform_count ); 
	for(int i=0; i<uniform_count; ++i)  {
		int name_len=-1, num=-1;
		GLenum type = GL_ZERO;
		char name[NGL_SHADER_UNIFORM_NAME_MAX_LEN]; //uniform is actually 0 terminated, but whatever, let's introduce 64 chars max
		glGetActiveUniform( shaderprogramID, GLuint(i), sizeof(name)-1,&name_len, &num, &type, name );
		name[name_len] = 0;
		GLuint location = glGetUniformLocation( shaderprogramID, name );

		nglShaderUniform* uniform = new nglShaderUniform(name,type);
		uniform->SetLocation(location);
		shaderprog->AddUniform(uniform);
	}

	return Result;
}

bool nglShaderCompiler::CompileShader(nglShader* shader, GLuint* out_shader_id)
{
	if  (shader->ShaderCodeAvaliable()){

		GLint compiledshader = glCreateShader(shader->GetShaderType());

		*out_shader_id = compiledshader;

		std::string shadercode = shader->GetShaderCode();

		char const * sourceptr = shadercode.c_str();

		glShaderSource(compiledshader, 1, &sourceptr , NULL);
		glCompileShader(compiledshader); 

		// Check Shader
		GLint Result = GL_FALSE;
		glGetShaderiv(compiledshader, GL_COMPILE_STATUS, &Result);
		if ( !Result ){

			*out_shader_id = NGL_SHADER_ERROR;

			int InfoLogLength;
			glGetShaderiv(compiledshader, GL_INFO_LOG_LENGTH, &InfoLogLength);
			std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
			glGetShaderInfoLog(compiledshader, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
			printf("nglRenderer::CompileShader() : %s\n", &VertexShaderErrorMessage[0]);
		}

		return Result;
	}

	return false;
}
