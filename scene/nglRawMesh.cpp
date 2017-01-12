#include "nglRawMesh.h"



ngl::scene::RawMesh::RawMesh()
{
	this->indices = NULL;
	this->uvCoords = NULL;
	this->vertices = NULL;
	this->normals = NULL;
	this->colors = NULL;
	this->_drawType = GL_TRIANGLES;
}

ngl::scene::RawMesh::~RawMesh()
{
	DeleteBuffers();
}

void ngl::scene::RawMesh::SetDrawType(GLenum val)
{
	_drawType = val;
}

GLenum ngl::scene::RawMesh::GetDrawType()
{
	return _drawType;
}

nglBufferObject<nglVertex3P>* ngl::scene::RawMesh::GetBufferVertices()
{
	return vertices;
}

nglBufferObject<unsigned short>* ngl::scene::RawMesh::GetBufferIndices()
{
	return indices;
}

nglBufferObject<nglVec2>* ngl::scene::RawMesh::GetBufferUV()
{
	return uvCoords;
}

nglBufferObject<nglVertex3P>* ngl::scene::RawMesh::GetBufferNormals()
{
	return normals;
}

void ngl::scene::RawMesh::SetBufferVertices(nglBufferObject<nglVertex3P>* buf)
{
	this->vertices = buf;
}

void ngl::scene::RawMesh::SetBufferIndices(nglBufferObject<unsigned short>* buf)
{
	this->indices = buf;
}

void ngl::scene::RawMesh::SetBufferUV(nglBufferObject<nglVec2>* buf)
{
	this->uvCoords = buf;
}

void ngl::scene::RawMesh::SetBufferNormals(nglBufferObject<nglVertex3P>* buf)
{
	this->normals = buf;
}

GLsizei ngl::scene::RawMesh::GetVertexCount()
{
	if (this->vertices != NULL){

		return this->vertices->CountElements();

	} else return 0;
}

GLsizei ngl::scene::RawMesh::GetIndexCount()
{
	if (this->indices != NULL){

		return this->indices->CountElements();

	} else return 0;
}

void ngl::scene::RawMesh::DeleteBuffers()
{
	if (vertices)
		delete vertices;
	if (indices)
		delete indices;
	if (uvCoords)
		delete uvCoords;
	if (normals)
		delete normals;
}

void ngl::scene::RawMesh::ClearBuffers()
{
	if (vertices)
		vertices->Clear();
	if (indices)
		indices->Clear();
	if (uvCoords)
		uvCoords->Clear();
	if (normals)
		normals->Clear();
}

nglBufferObject<nglRGB3F>* ngl::scene::RawMesh::GetBufferColors()
{
	return colors;
}

void ngl::scene::RawMesh::SetBufferColors(nglBufferObject<nglRGB3F>* buf)
{
	colors = buf;
}

