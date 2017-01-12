#include "nglRenderer.h"
#include "nglMesh.h"
#include "nglGeometry.h"
#include "nglShaderGlobals.h"
#include "detail\type_mat.hpp"
#include "gtc\matrix_transform.inl"
#include "glm/glm.hpp"
#include "nglTexture.h"
#include <iostream>
#include <string>
#include "nglShaderUniform.h"
#include "nglShaderProgram.h"
#include "nglCommon.h"
#include "../scene/nglTerrain.h"


//#define NGL_RENDERER_DEBUG 1

nglRenderer::nglRenderer(nglMaterialManager* matman)
{

}

nglRenderer::~nglRenderer()
{

}

void nglRenderer::Render(nglScene* pScene)
{
	glEnable (GL_DEPTH_TEST);

	this->Clear();

	_cam = pScene->GetActiveCamera();

	/*

	We want to render close up geometry first
	That way, gpu can cull away geometry which is obstructed by things that are in front of it from cam perspective

	*/

	/************************************************************************/
	/* SCENE GEOMETRY                                                       */
	/************************************************************************/

	_rq_geometry.render_meshes.clear();

	std::vector<nglGeometry*> geometry_nodes = pScene->GetChildren<nglGeometry>(NGL_SCENENODE_TYPE_GEOMETRY);

	for (unsigned int i = 0; i < geometry_nodes.size();i++)
	{
		_rq_geometry.AddMeshesFromContainer(geometry_nodes[i]);
	}


	/************************************************************************/
	/* SCENE TERRAIN                                                        */
	/************************************************************************/

	_rq_terrain.render_meshes.clear();

	std::vector<nglTerrain*> terrain_nodes = pScene->GetChildren<nglTerrain>(NGL_SCENENODE_TYPE_TERRAIN);

	for (unsigned int i = 0; i < terrain_nodes.size();i++)
	{
		_rq_terrain.AddMeshesFromContainer(terrain_nodes[i]);
	}

	//lights
	std::vector<nglLight*> light_nodes = pScene->GetChildren<nglLight>(NGL_SCENENODE_TYPE_LIGHT);

	if (_renderBV){

		for (unsigned int i = 0; i < geometry_nodes.size();i++)
		{
			_rq_boundingvolumes.AddBoundingVolumeMeshesFromContainer(geometry_nodes[i]);

		}

		for (unsigned int i = 0; i < terrain_nodes.size();i++){

			_rq_boundingvolumes.AddBoundingVolumeMeshesFromContainer(terrain_nodes[i]);
		}

	}



	for (unsigned int i = 0; i < _render_passes.size();i++)
	{
		nglRenderPass* pass = _render_passes[i];

		glClear(pass->GetClearBits());

		for (unsigned int e = 0; e < _renderQueue.GetListCount();e++)
		{
			RenderQueueList* renderlist = _renderQueue.GetList(e);
			this->RenderPass(renderlist,pass);

		}

	}



	//delete renderqueue
	_renderQueue.Clear();

}


bool nglRenderer::RenderDepthMap(nglLight* light)
{

	nglRenderLightObject* render_light = this->MapLight(light);

	//bind dramebuffer, we will draw to this!
	this->_context.BindParameter<GLuint>(NGL_RENDERCONTEXT_PARAM_TYPE_FRAMEBUFFER,render_light->framebuffer);

	_depth_texture_pass->SetFramebuffer(render_light->framebuffer, GL_NONE); //render depth to main window
	_depth_texture_pass->SetRenderObserver(light);

	//now we start rendering.
	//for rendering a depth map, we can cull the triangles!
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles

	//this->RenderPass()


	//I need to write a rendering method which can work for both normal rendering, and depth rendering
	//it should support setting what framebuffer to write to
	//it should get shader and framebuffer as input, and then render
	//there should be a renderpass class, which encapsulates every one of these render passes
	//render passes are, for example, the depth buffer generation we are doing above



}

void nglRenderer::RenderPass(nglRenderPass* pass_parameter, ngl::render::RenderQueue& queue)
{

	nglMaterial* mat = pass_parameter->GetRenderMaterial();

	//setup the framebuffer that we want to render to
	this->_context.BindParameter<GLint>(NGL_RENDERCONTEXT_PARAM_TYPE_FRAMEBUFFER,pass_parameter->GetFrameBuffer());

	nglVec2U16 resolution = pass_parameter->GetResolution();
	nglVec2U16 orgin = pass_parameter->GetOrgin();
	glViewport(orgin.x,orgin.y,resolution.x,resolution.y);


	if (mat->IsWiremesh())
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (!this->EnableMaterial(mat))
	{
		printf("nglRenderer::RenderPass() : Can't enable material! \n");
		return;
	}


}

void nglRenderer::RenderList(RenderQueueList* renderlist)
{
	//glFrontFace(GL_CW);
	//glEnable (GL_DEPTH_TEST);



	nglShaderProgram* shaderprog = mat->GetShaderProgram();

	//Get shader uniforms, which are from material definition
	std::vector<nglShaderUniform*> uniform_array = shaderprog->GetUniforms();

	//used later on when rendering! We just piggypack from the following loop
	nglShaderUniform* uniform_model_transformation = NULL;

	//handle shader uniforms!
	for (unsigned int i = 0; i < uniform_array.size();i++)
	{
		nglShaderUniform* uniform = uniform_array[i];

		if (uniform->GetUsage() == NGL_SHADER_UNIFORM_USAGE_MODEL_TRANSFORMATION)
			uniform_model_transformation = uniform;

		if (uniform->GetLocation() == nglShaderUniform::LOC_UNKNOWN){

			GLint uniform_location = glGetUniformLocation(shaderprog->GetProgram(),uniform->GetName().c_str());

			if (uniform_location == -1){
				//uniform actually doesn't exist!
				//we set it to not defined, so it is ignored.
				uniform->SetLocation(nglShaderUniform::LOC_NOT_DEFINED);
				printf("nglRenderer::RenderList() : Shader defined in material, but not in shader! \n");
			} else {

				uniform->SetLocation(uniform_location);
			}

		}

		switch(uniform->GetUsage()){

			//we handle everything except the model transformation uniform here
			//that one can change for each mesh, so we do it directly before rendering each mesh :)

		case NGL_SHADER_UNIFORM_USAGE_CAMERA_VP:
			//set camera VP!
			{
				glm::mat4 vp = pass_parameter->GetRenderObserver()->GetProjectionMatrix() * pass_parameter->GetRenderObserver()->GetViewMatrix();
				glUniformMatrix4fv(uniform->GetLocation(), 1, false, (GLfloat*)glm::value_ptr(vp));
			}
			break;

		case NGL_SHADER_UNIFORM_USAGE_UNDEFINED:
			//set whatever parameters are defined in material definition, and set the parameters from nglMaterial!
			void* data = NULL;

			data = mat->GetMaterialParameter(uniform->GetName());

			if (data != NULL) {

				//alright! We have a parameter for a uniform!
				//now we need to find out what type it is!
				switch (uniform->GetType()) {

				case NGL_SHADER_UNIFORM_TYPE_MAT4:
					glUniformMatrix4fv(uniform->GetLocation(), 1, false, (GLfloat*)data);
					break;

				case NGL_SHADER_UNIFORM_TYPE_VEC3:
					glUniform3fv(uniform->GetLocation(), 1, (GLfloat*)data);
					break;

				case NGL_SHADER_UNIFORM_TYPE_TEXTURE2D:
					glUniform1i(uniform->GetLocation(), 0);
					break;

				default:
					printf("nglRenderer::RenderList() : Unknown shader uniform type! \n");
					break;

				}


			} //end (data != NULL) 
			else {
				printf("nglRenderer::RenderList() : Uniform defined in material definition, but no data bound to it in material! uniform_name=%s \n", uniform->GetName().c_str());

			}

			break;


		} // end switch(uniform->GetUsage())

	} //end of uniform handling


	if (uniform_model_transformation != NULL)
	{

		//now, we can finally render our shit
		for (unsigned int i = 0; i < renderlist->meshes.size();i++)
		{
			RenderQueueEntryMesh mesh_entry = renderlist->meshes[i];

			//send mesh transform to uniform!
			glm::mat4 final_transform =  mesh_entry.world_transform * mesh_entry.local_transform;
			glUniformMatrix4fv(uniform_model_transformation->GetLocation(), 1, false, (GLfloat*)glm::value_ptr(final_transform));

			nglRenderMeshObject* render_object = this->MapMesh(mesh_entry.mesh,mat);

			this->_context.BindParameter<GLint>(NGL_RENDERCONTEXT_PARAM_TYPE_VAO,render_object->vao);


			if (mat->HasTexture()){

				nglTexture* active_tex = mat->GetActiveTexture();

				//this->MapTexture(active_tex);
				this->_context.BindParameter<GLint>(NGL_RENDERCONTEXT_PARAM_TYPE_ACTIVE_TEXTURE_UNIT,GL_TEXTURE0 + 0);

				//GLuint TextureID = active_tex->GetTextureObjectID();
				//this->_context.BindParameter<GLuint>(NGL_RENDERCONTEXT_PARAM_TYPE_BOUNDTEXTURE,TextureID);

			} else {

				this->_context.BindParameter<GLuint>(NGL_RENDERCONTEXT_PARAM_TYPE_BOUNDTEXTURE,0);

			}

			if (mesh_entry.mesh->GetBufferIndices() != NULL)
				glDrawElements(mesh_entry.mesh->GetDrawType(),mesh_entry.mesh->GetIndexCount(), GL_UNSIGNED_SHORT ,0);
			else
				glDrawArrays(mesh_entry.mesh->GetDrawType(), 0, mesh_entry.mesh->GetVertexCount()); 

			this->_context.BindParameter<GLint>(NGL_RENDERCONTEXT_PARAM_TYPE_VAO,0);

		} 

	} // end (uniform_model_transformation != NULL)

}









void nglRenderer::Clear()
{
	//glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.3,0.3,0.3,1.0);
}







void nglRenderer::AddRenderPass(nglRenderPass* renderpass)
{
	if (renderpass)
		_render_passes.push_back(renderpass);
}

void nglRenderer::RemoveRenderPass(int indx)
{
	//todo : delete
}


