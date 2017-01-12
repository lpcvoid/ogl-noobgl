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
#include "nglLight.h"
#include "nglRenderPass.h"

#ifndef nglRendererBase_h__
#define nglRendererBase_h__

using namespace ngl;
using namespace scene;
using namespace render;


struct nglRenderMeshObject {

	GLuint vao;
	std::map<NGL_ARRAY_DATA_TYPE,GLuint> buffers;

	GLuint GetBuffer(NGL_ARRAY_DATA_TYPE type);

	~nglRenderMeshObject();
	nglRenderMeshObject();
};

struct nglRenderTextureObject {

	GLuint texture;
	GLint texture_unit;

	~nglRenderTextureObject();
	nglRenderTextureObject();

};

struct nglRenderLightObject {

	nglRenderTextureObject* depth_map;
	//used only for rendering from light perspective
	GLuint framebuffer;
	//framebuffer okay
	bool framebuffer_okay;

	~nglRenderLightObject();
	nglRenderLightObject();

};

class nglRendererBase {

public:

	nglRendererBase();
	virtual ~nglRendererBase();

	virtual void Render(nglScene* pScene) = 0;

protected:

	nglRenderContext _context;
	nglShaderCompiler _shadercompiler;

	/************************************************************************/
	/* RENDER MAPPINGS                                                      */
	/************************************************************************/

	//this links raw meshes to gpu vao and buffers for rendering
	std::map<RawMesh*, nglRenderMeshObject*> _render_map;
	//this links nglTexture objects to gpu texture
	std::map<nglTexture*, nglRenderTextureObject*> _texture_map;
	//this also links lights to their respective depth maps and framebuffers
	std::map<nglLight*, nglRenderLightObject*> _light_map;
	//this links nglShaderProgram to gluint describing gl shader
	std::map<nglShaderProgram*, GLuint> _shaderprogram_map;

	//map a render object from a mesh
	nglRenderMeshObject* MapMesh(RawMesh* mesh);
	//map a render texture from a ngltexture
	nglRenderTextureObject* MapTexture(nglTexture* texture);
	//map a light to a render light
	nglRenderLightObject* MapLight(nglLight* light);
	//map a shader to a gl shader
	GLuint MapShaderProgram(nglShaderProgram* shaderprog);




	/************************************************************************/
	/* RENDER PASSES                                                        */
	/************************************************************************/

// 	//passes which the renderer runs through
// 	std::vector<nglRenderPass*> _render_passes;
// 
// 	//terrain render pass
// 	nglRenderPass* _rp_terrain;
// 	//world geometry render pass
// 	nglRenderPass* _rp_geometry;
// 	//water render pass
// 	//nglRenderPass* _rp_water;
// 
// 	void RenderPass(nglRenderPass* pass_parameter, ngl::render::RenderQueue& queue);
// 	void RenderList(ngl::render::RenderQueue& render_queue);

	/************************************************************************/
	/* RENDER QUEUES                                                        */
	/************************************************************************/

// 	//renderqueue for geometry
// 	ngl::render::RenderQueue _rq_geometry;
// 	//renderqueue for terrain
// 	ngl::render::RenderQueue _rq_terrain;
// 	//renderqueue for bounding volumes
// 	ngl::render::RenderQueue _rq_boundingvolumes;


	/************************************************************************/
	/* RENDER HELPERS                                                       */
	/************************************************************************/

	//uploads raw data to gpu. Returns the vbo ID of the uploaded data.
	GLuint BufferDataToVRAM(void* buffer, int byte_len, NGL_ARRAY_DATA_TYPE type);
	bool EnableShaderProgram(nglShaderProgram* shaderprog);

	//set vertex attributes for this mesh
	bool BindShaderToMesh(ngl::Mesh* mesh, nglShaderProgram* shader);

};

#endif // nglRendererBase_h__