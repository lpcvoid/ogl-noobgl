#include <iostream>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include <vector>
#include "glm/fwd.hpp"
#include "glm/detail/type_vec.hpp"
#include "nglScene.h"
#include "nglSceneNode.h"
#include "nglGeometry.h"
#include "nglCamera.h"
#include "nglShaderProgram.h"
#include "nglTexture.h"
#include "nglRenderContext.h"
#include "nglShaderCompiler.h"
#include <map>
#include <nglMesh.h>
#include <nglRawMesh.h>
#include "nglMaterial.h"
#include "nglRenderQueue.h"
#include "../scene/nglLight.h"
#include "nglRenderPass.h"

#ifndef nglRenderer_h__
#define nglRenderer_h__




class nglRenderer{

public:
	nglRenderer();
	~nglRenderer();
	void Render(nglScene* pScene);
	void Clear();

	void AddRenderPass(nglRenderPass* renderpass);
	void RemoveRenderPass(int indx);

private:




	//material for boundingvolumes
	nglMaterial* _bv_material;

	bool _renderBV;

	//shadowmap stuff, TEST
	bool _enable_shadowmapping;
	nglMaterial* _sm_depth_pass;
	nglMaterial* _sm_depth_material; //depth buffer from POV of light
	nglRenderPass* _depth_texture_pass;
	bool RenderDepthMap(nglLight* light);






	
};


#endif // nglRenderer_h__