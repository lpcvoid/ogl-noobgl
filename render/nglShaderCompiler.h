#include "raxLogging.h"
#include "nglShaderProgram.h"
#include <GL/glew.h>

#ifndef nglShaderCompiler_h__
#define nglShaderCompiler_h__

class nglShaderCompiler {

public:
	nglShaderCompiler(rax::logging::Logging* logger);
	nglShaderCompiler();
	bool CompileShaderProgram(nglShaderProgram* shaderprog, GLuint* out_shader_id);

private:
	rax::logging::Logging* _logger;
	bool CompileShader(nglShader* shader, GLuint* out_shader_id);

};

#endif // nglShaderCompiler_h__