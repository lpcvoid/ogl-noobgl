#include "nglRenderPass.h"
#include "raxBits.h"




nglRenderPass::nglRenderPass()
{
	_rendermode = NGL_RENDERPASS_RENDERMODE_NORMAL;
	_depth_shader = NULL;
	_framebuffer = -1;
	_org_x = 0;
	_org_y = 0;
	_clearbits = 0;
}

nglRenderPass::~nglRenderPass()
{

}

void nglRenderPass::SetFramebuffer(GLuint fb, GLenum color_output)
{
	_framebuffer = fb;
}
GLuint nglRenderPass::GetFrameBuffer()
{
	return _framebuffer;
}

void nglRenderPass::SetResolution(int width, int height)
{
	_res_y = height;
	_res_x = width;
}

void nglRenderPass::SetRenderMode(NGL_RENDERPASS_RENDERMODE rendermode)
{
	_rendermode = rendermode;
}

NGL_RENDERPASS_RENDERMODE nglRenderPass::GetRenderMode()
{
	return _rendermode;
}

void nglRenderPass::SetRenderMaterial(nglMaterial* mat)
{
	_depth_shader = mat;
}

nglMaterial* nglRenderPass::GetRenderMaterial()
{
	return _depth_shader;
}

nglVec2U16 nglRenderPass::GetResolution()
{
	nglVec2U16 res;
	res.x = _res_x;
	res.y = _res_y;
	return res;
}

void nglRenderPass::SetOrgin(int x, int y)
{
	_org_x = x;
	_org_y = y;
}

nglVec2U16 nglRenderPass::GetOrgin()
{
	nglVec2U16 res;
	res.x = _org_x;
	res.y = _org_y;
	return res;
}

void nglRenderPass::SetClearBit(unsigned int bitmask)
{
	_clearbits = bitmask;
}

void nglRenderPass::DeleteClearBits()
{
	_clearbits = 0;
}

unsigned int nglRenderPass::GetClearBits()
{
	return _clearbits;
}

void nglRenderPass::SetRenderObserver(nglSceneObserver* obs)
{
	_observer = obs;
}

nglSceneObserver* nglRenderPass::GetRenderObserver()
{
	return _observer;
}
