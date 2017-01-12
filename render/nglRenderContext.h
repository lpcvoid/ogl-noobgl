#include <GL/glew.h>

#ifndef nglRenderContext_h__
#define nglRenderContext_h__

enum NGL_RENDERCONTEXT_PARAM_TYPE {

	NGL_RENDERCONTEXT_PARAM_TYPE_VAO,
	NGL_RENDERCONTEXT_PARAM_TYPE_SHADERPROGRAM,
	NGL_RENDERCONTEXT_PARAM_TYPE_ACTIVE_TEXTURE_UNIT,
	NGL_RENDERCONTEXT_PARAM_TYPE_BOUNDTEXTURE,
	NGL_RENDERCONTEXT_PARAM_TYPE_FRAMEBUFFER,
	NGL_RENDERCONTEXT_PARAM_TYPE_FRAMEBUFFER_TEXTURE_DEPTH_ATTACHMENT,
	NGL_RENDERCONTEXT_PARAM_TYPE_FRAMEBUFFER_COLOR_OUTPUT

};

class nglRenderContext {

public:
	template <typename T>
	void BindParameter(NGL_RENDERCONTEXT_PARAM_TYPE type, T value );

	nglRenderContext();

private:
	GLuint _currentVAO;
	GLint _currentShaderProgram;
	GLint _currentActiveTextureUnit; //TEXTURE0..TEXTURE15
	GLuint _currentBoundTexture;
	GLuint _currentBoundFrameBuffer;
	GLenum _currentFramebufferColorOutput; //glDrawBuffer() parameter
	GLuint _currentFramebufferDepthAttachment; // glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, ***, 0);
};

template <typename T>
void nglRenderContext::BindParameter(NGL_RENDERCONTEXT_PARAM_TYPE type, T value)
{
	switch(type){

	case NGL_RENDERCONTEXT_PARAM_TYPE_ACTIVE_TEXTURE_UNIT:
		if (value != _currentActiveTextureUnit){
			glActiveTexture(value);
			_currentActiveTextureUnit = value;
		}
		break;

	case NGL_RENDERCONTEXT_PARAM_TYPE_BOUNDTEXTURE:
		if (value != _currentBoundTexture){
			glBindTexture(GL_TEXTURE_2D, value);
			_currentBoundTexture = value;
		}
		break;

	case NGL_RENDERCONTEXT_PARAM_TYPE_SHADERPROGRAM:
		if (value != _currentShaderProgram){
			glUseProgram(value);
			_currentShaderProgram = value;
		}
		break;

	case NGL_RENDERCONTEXT_PARAM_TYPE_VAO:
		if (value != _currentVAO){
			glBindVertexArray(value);
			_currentVAO = value;
		}
		break;

	case NGL_RENDERCONTEXT_PARAM_TYPE_FRAMEBUFFER:
		if (value != _currentBoundFrameBuffer){
			glBindFramebuffer(GL_FRAMEBUFFER,value);
			_currentBoundFrameBuffer = value;
		}
		break;

	case NGL_RENDERCONTEXT_PARAM_TYPE_FRAMEBUFFER_COLOR_OUTPUT:
		if (value != _currentFramebufferColorOutput){
			glDrawBuffer(value);
			_currentFramebufferColorOutput = value;
		}
		break;

	case NGL_RENDERCONTEXT_PARAM_TYPE_FRAMEBUFFER_TEXTURE_DEPTH_ATTACHMENT:
		if (value != _currentFramebufferDepthAttachment){
			glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,value,0);
			_currentFramebufferDepthAttachment = value;
		}
		break;

	}
}

#endif // nglRenderContext_h__

