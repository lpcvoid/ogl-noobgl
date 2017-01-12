#include "nglRenderContext.h"

nglRenderContext::nglRenderContext()
{
	this->_currentActiveTextureUnit = -1;
	this->_currentBoundTexture = -1;
	this->_currentShaderProgram = -1;
	this->_currentVAO = -1;
	this->_currentBoundFrameBuffer = -1;
}

