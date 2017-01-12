#include "nglRendererBase.h"
#include "nglShaderGlobals.h"
#include "glm/glm.hpp"

nglRenderMeshObject* nglRendererBase::MapMesh(RawMesh* mesh)
{


	nglRenderMeshObject* render_object;

	auto mesh_it = this->_render_map.find(mesh);

	if (mesh_it != this->_render_map.end()){

		//first = key
		//second = value
		render_object = mesh_it->second;

	} else {

		render_object = new nglRenderMeshObject();
		//create vertex array object, to which we bind a VBO later.
		//the vao saves state info and info about the format of the VBO.
		//see : http://stackoverflow.com/questions/23314787/use-of-vertex-array-objects-and-vertex-buffer-objects
		glGenVertexArrays(1, &render_object->vao);
		//bind vao
		this->_context.BindParameter<GLuint>(NGL_RENDERCONTEXT_PARAM_TYPE_VAO,render_object->vao);
		//now we handle the buffers.
		//vertex
		nglBufferObject<nglVertex3P>* buf_vertices = mesh->GetBufferVertices();

		if (buf_vertices != NULL){
			GLuint vbo = this->BufferDataToVRAM(buf_vertices->GetBufferData(),buf_vertices->GetDataLength(),NGL_ARRAY_DATA_TYPE_VERTEX);
			render_object->buffers[NGL_ARRAY_DATA_TYPE_VERTEX] = vbo;
#ifdef NGL_RENDERER_DEBUG
			printf("VBO : mesh : vertex\n");
#endif
		}

		//index, always uploaded, if it is avaliable.
		nglBufferObject<unsigned short>* buf_index = mesh->GetBufferIndices();
		if (buf_index != NULL){
			GLuint vbo = this->BufferDataToVRAM(buf_index->GetBufferData(),buf_index->GetDataLength(),NGL_ARRAY_DATA_TYPE_INDEX);
			render_object->buffers[NGL_ARRAY_DATA_TYPE_INDEX] = vbo;
#ifdef NGL_RENDERER_DEBUG
			printf("VBO : mesh : index\n");
#endif
		}

		//uv
		nglBufferObject<nglVec2>* buf_uv = mesh->GetBufferUV();
		if (buf_uv != NULL){
			GLuint vbo = this->BufferDataToVRAM(buf_uv->GetBufferData(),buf_uv->GetDataLength(),NGL_ARRAY_DATA_TYPE_UV);
			render_object->buffers[NGL_ARRAY_DATA_TYPE_UV] = vbo;
#ifdef NGL_RENDERER_DEBUG
			printf("VBO : mesh : uv\n");
#endif
		}

		//normals
		nglBufferObject<nglVertex3P>* buf_normals = mesh->GetBufferNormals();
		if (buf_normals != NULL){
			GLuint vbo = this->BufferDataToVRAM(buf_normals->GetBufferData(),buf_normals->GetDataLength(),NGL_ARRAY_DATA_TYPE_NORMALS);
			render_object->buffers[NGL_ARRAY_DATA_TYPE_NORMALS] = vbo;
#ifdef NGL_RENDERER_DEBUG
			printf("VBO : mesh : normals\n");
#endif
		}
		//colors
		nglBufferObject<nglRGB3F>* buf_colors = mesh->GetBufferColors();

		if (buf_colors != NULL ){
			GLuint vbo = this->BufferDataToVRAM(buf_colors->GetBufferData(),buf_colors->GetDataLength(),NGL_ARRAY_DATA_TYPE_COLORS);
			render_object->buffers[NGL_ARRAY_DATA_TYPE_COLORS] = vbo;
#ifdef NGL_RENDERER_DEBUG
			printf("VBO : mesh : colors\n");
#endif
		}

		mesh->SetDirty(false);
		this->_render_map[mesh] = render_object;
	}
	return render_object;

}

GLuint nglRendererBase::BufferDataToVRAM(void* buffer, int byte_len, NGL_ARRAY_DATA_TYPE type)
{

	GLuint vbo;
	glGenBuffers(1, &vbo);

	switch(type){

	case NGL_ARRAY_DATA_TYPE_UV:
	case NGL_ARRAY_DATA_TYPE_VERTEX:
	case NGL_ARRAY_DATA_TYPE_NORMALS:
	case NGL_ARRAY_DATA_TYPE_COLORS:
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, byte_len, buffer, GL_STATIC_DRAW);
		break;

	case NGL_ARRAY_DATA_TYPE_INDEX:
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, byte_len, buffer, GL_STATIC_DRAW);
		break;

	}

	return vbo;

}

nglRenderTextureObject* nglRendererBase::MapTexture(nglTexture* texture)
{

	nglRenderTextureObject* render_texture;

	auto tex_it = this->_texture_map.find(texture);

	if (tex_it != this->_texture_map.end()){

		//first = key
		//second = value
		render_texture = tex_it->second;

	} else {
		//doesn't exist. we need to create it!
		render_texture = new nglRenderTextureObject();
		glGenTextures(1,&render_texture->texture);
		this->_context.BindParameter<GLuint>(NGL_RENDERCONTEXT_PARAM_TYPE_BOUNDTEXTURE,render_texture->texture);

		//load the texture, in case it hasn't been loaded already.
		//if there is no image path set, this does nothing.
		//for shadow maps, this does nothing (see above)
		texture->Load();

		//todo - how do I find out pixel alignment of textures?
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, texture->GetTextureInternalFormat(), texture->GetImageWidth(), texture->GetImageHeight(), 0, texture->GetTextureFormat(),texture->GetTextureDataType(), texture->GetImageData());
		//set texture properties
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture->GetMinMagFilter());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture->GetMinMagFilter());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture->GetWrapMode());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture->GetWrapMode());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, texture->GetTextureCompareFunc());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, texture->GetTextureCompareMode());

		//this->_context.BindParameter<GLuint>(NGL_RENDERCONTEXT_PARAM_TYPE_BOUNDTEXTURE,0);

		_texture_map[texture] = render_texture;

	}

	return render_texture;
}

GLuint nglRendererBase::MapShaderProgram(nglShaderProgram* shaderprog)
{
	GLuint shader_program = -1;

	auto shader_it = this->_shaderprogram_map.find(shaderprog);

	if (shader_it != this->_shaderprogram_map.end()){

		//first = key
		//second = value
		shader_program = shader_it->second;

	} else {

		bool result = this->_shadercompiler.CompileShaderProgram(shaderprog,&shader_program);

		if (!result)
			printf("nglRenderer::MapShaderProgram() : Couldn't compile shader! \n");

		_shaderprogram_map[shaderprog] = shader_program;

	}

	return shader_program;
}

bool nglRendererBase::EnableShaderProgram(nglShaderProgram* shaderprog)
{

	GLuint shader_id = this->MapShaderProgram(shaderprog);

	if ( shader_id != NGL_SHADER_ERROR){

		this->_context.BindParameter<GLuint>(NGL_RENDERCONTEXT_PARAM_TYPE_SHADERPROGRAM,shader_id);
		return true;

	} else {
		return false;
	}
}

nglRenderLightObject* nglRendererBase::MapLight(nglLight* light)
{
	nglRenderLightObject* render_light;

	auto light_it = this->_light_map.find(light);
	if (light_it != this->_light_map.end()){

		//first = key
		//second = value
		render_light = light_it->second;

	} else {
		//doesn't exist. we need to create it!

		render_light = new nglRenderLightObject();
		//generate framebuffer
		glGenFramebuffers(1, &render_light->framebuffer);
		//activate framebuffer
		this->_context.BindParameter<GLuint>(NGL_RENDERCONTEXT_PARAM_TYPE_FRAMEBUFFER,render_light->framebuffer);

		//now we generate an empty texture which will later contain our depth map for that light
		render_light->depth_map = this->MapTexture(light->GetDepthMap());

		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, render_light->depth_map->texture, 0);

		// No color output in the bound framebuffer, only depth.
		glDrawBuffer(GL_NONE);

		// Always check that our framebuffer is ok
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			return render_light;

		render_light->framebuffer_okay = true;

		this->_context.BindParameter<GLuint>(NGL_RENDERCONTEXT_PARAM_TYPE_FRAMEBUFFER,0);

		_light_map[light] = render_light;
	}

	return render_light;
}

nglRendererBase::nglRendererBase()
{
// 	_matman = matman;
// 	_bv_material = matman->GetMaterial(std::string("boundingvolume"));
// 	_bv_material->SetVec3(std::string("color"),glm::vec3(1,0,0));
// 	_renderBV = true;
// 
// 
// 	_enable_shadowmapping = true;
// 
// 	//load depth pass material
// 	_sm_depth_pass = matman->GetMaterial(std::string("pass_depth"));
// 
// 	_depth_texture_pass = new nglRenderPass();
// 	//_depth_texture_pass->SetFramebuffer(0, GL_FRONT_AND_BACK); //render depth to main window
// 	//_depth_texture_pass->SetRenderObserver(sun_light);
// 	_depth_texture_pass->SetRenderMaterial(_sm_depth_pass);
// 	_depth_texture_pass->SetRenderMode(NGL_RENDERPASS_RENDERMODE_DEPTH);
// 	_depth_texture_pass->SetResolution(NGL_RENDER_SHADOWMAP_RES,NGL_RENDER_SHADOWMAP_RES);
// 	_depth_texture_pass->SetClearBit(GL_DEPTH_BUFFER_BIT);
// 	_depth_texture_pass->SetOrgin(0,0);

	glEnable(GL_DEPTH_TEST);
}

nglRendererBase::~nglRendererBase()
{

}

bool nglRendererBase::BindShaderToMesh(ngl::Mesh* mesh, nglShaderProgram* shader)
{
	//bind vertex attributes for every data source mesh has
	EnableShaderProgram(shader);

	nglRenderMeshObject* rmo = this->MapMesh(mesh); 



	nglBufferObject<nglVertex3P>* buf_vertices = mesh->GetBufferVertices();
	if (buf_vertices != NULL && shader->GetInputFlag(buf_vertices->GetBufferType())){
		glEnableVertexAttribArray(NGL_SHADER_LOCATION_IN_VERTEX_VERTEXDATA);
		GLuint buffer = rmo->GetBuffer((NGL_ARRAY_DATA_TYPE_VERTEX));
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glVertexAttribPointer(NGL_SHADER_LOCATION_IN_VERTEX_VERTEXDATA, 3, GL_FLOAT, GL_FALSE,buf_vertices->GetStride(), (void*)buf_vertices->GetOffset());
	}
	//uv
	nglBufferObject<nglVec2>* buf_uv = mesh->GetBufferUV();
	if (buf_uv != NULL && shader->GetInputFlag(buf_uv->GetBufferType())){
		glEnableVertexAttribArray(NGL_SHADER_LOCATION_IN_VERTEX_UVCOORDS);
		GLuint buffer = rmo->GetBuffer((NGL_ARRAY_DATA_TYPE_UV));
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glVertexAttribPointer(NGL_SHADER_LOCATION_IN_VERTEX_UVCOORDS, 2, GL_FLOAT, GL_FALSE,buf_uv->GetStride(), (void*)buf_uv->GetOffset());
	}
	//normals
	nglBufferObject<nglVertex3P>* buf_normals = mesh->GetBufferNormals();
	if (buf_normals != NULL && shader->GetInputFlag(buf_normals->GetBufferType())){
		glEnableVertexAttribArray(NGL_SHADER_LOCATION_IN_VERTEX_NORMALS);
		GLuint buffer = rmo->GetBuffer((NGL_ARRAY_DATA_TYPE_NORMALS));
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glVertexAttribPointer(NGL_SHADER_LOCATION_IN_VERTEX_NORMALS, 3, GL_FLOAT, GL_FALSE,buf_normals->GetStride(), (void*)buf_normals->GetOffset());
	}
	//colors
	nglBufferObject<nglRGB3F>* buf_colors = mesh->GetBufferColors();
	if (buf_colors != NULL && shader->GetInputFlag(buf_colors->GetBufferType()) ){
		glEnableVertexAttribArray(NGL_SHADER_LOCATION_IN_VERTEX_COLORS);
		GLuint buffer = rmo->GetBuffer((NGL_ARRAY_DATA_TYPE_COLORS));
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glVertexAttribPointer(NGL_SHADER_LOCATION_IN_VERTEX_COLORS, 3, GL_FLOAT,GL_FALSE,buf_colors->GetStride(), (void*)buf_colors->GetOffset());
	}

	return true;

}



nglRenderMeshObject::~nglRenderMeshObject()
{

	//delete buffers from gpu!
	glDeleteVertexArrays(1,&vao);

	for(auto iterator = buffers.begin(); iterator != buffers.end(); iterator++) {
		// iterator->first = key
		// iterator->second = value
		glDeleteBuffers(1,&iterator->second);
	}
}

nglRenderMeshObject::nglRenderMeshObject()
{
	vao = UINT_MAX;
}

GLuint nglRenderMeshObject::GetBuffer(NGL_ARRAY_DATA_TYPE type)
{
	GLuint buf;

	auto it = this->buffers.find(type);
	if (it != this->buffers.end()){

		//first = key
		//second = value
		buf = it->second;

	} else buf = -1;

	return buf;
}

nglRenderTextureObject::~nglRenderTextureObject()
{
	if (texture != UINT_MAX)
		glDeleteTextures(1,&texture);
}

nglRenderTextureObject::nglRenderTextureObject()
{
	texture = UINT_MAX;
	texture_unit = INT_MAX;
}

nglRenderLightObject::~nglRenderLightObject()
{
	if (framebuffer != INT_MAX)
		glDeleteFramebuffers(1, &framebuffer);
}

nglRenderLightObject::nglRenderLightObject()
{
	depth_map = new nglRenderTextureObject();
	framebuffer = INT_MAX;
	framebuffer_okay = false;
}