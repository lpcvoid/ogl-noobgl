#include <iostream>
#include <GL/glew.h>
#include <vector>
#include "nglCommon.h"
#include "nglMaterial.h"
#include "nglSceneObserver.h"

#ifndef nglRenderPass_h__
#define nglRenderPass_h__

/*

Every render pass needs :

A target framebuffer, and target resolution
A material to use for rendering (or none, in that case the geometry material will be used)
An observer for rendering the scene from that perspective


*/

enum NGL_RENDERPASS_RENDERMODE {

	NGL_RENDERPASS_RENDERMODE_DEPTH = 1,
	NGL_RENDERPASS_RENDERMODE_NORMAL = 2,
	//NGL_RENDERPASS_RENDERMODE = 4

};

class nglRenderPass {

public:
	nglRenderPass();
	virtual ~nglRenderPass();

	void SetFramebuffer(GLuint fb, GLenum color_output);
	GLuint GetFrameBuffer();
	GLenum GetFramebufferColorOutput();

	void SetResolution(int width, int height);
	nglVec2U16 GetResolution();

	void SetOrgin(int x, int y);
	nglVec2U16 GetOrgin();

	void SetRenderObserver(nglSceneObserver* obs);
	nglSceneObserver* GetRenderObserver();


	void SetRenderMode(NGL_RENDERPASS_RENDERMODE rendermode);
	NGL_RENDERPASS_RENDERMODE GetRenderMode();

	void SetRenderMaterial(nglMaterial* mat);
	nglMaterial* GetRenderMaterial();

	void SetClearBit(unsigned int bitmask);
	void DeleteClearBits();
	unsigned int GetClearBits();

private:
	//target framebuffer
	GLuint _framebuffer;
	//viewport orgin
	int _org_x, _org_y;
	//viewport resolution
	int _res_x, _res_y;
	//clear parameters
	unsigned int _clearbits; 
	//render pass render mode
	NGL_RENDERPASS_RENDERMODE _rendermode;
	//depth pass shader. Only needed when render mode = depth
	nglMaterial* _depth_shader;
	//observer from which scene is to be rendered
	nglSceneObserver* _observer;

};

#endif // nglRenderPass_h__