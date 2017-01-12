#include <iostream>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include <vector>
#include "nglBufferObject.h"
#include <nglObject.h>


#ifndef nglRawMesh_h__
#define nglRawMesh_h__


namespace ngl {

	namespace scene {

		class RawMesh : public nglObject {

		public:




			RawMesh();
			virtual ~RawMesh();

			nglBufferObject<nglVertex3P>* GetBufferVertices();
			nglBufferObject<unsigned short>* GetBufferIndices();
			nglBufferObject<nglVec2>* GetBufferUV();
			nglBufferObject<nglVertex3P>* GetBufferNormals();
			nglBufferObject<nglRGB3F>* GetBufferColors();

			void SetBufferVertices( nglBufferObject<nglVertex3P>* buf);
			void SetBufferIndices( nglBufferObject<unsigned short>* buf);
			void SetBufferUV (nglBufferObject<nglVec2>* buf);
			void SetBufferNormals(nglBufferObject<nglVertex3P>* buf);
			void SetBufferColors(nglBufferObject<nglRGB3F>* buf);

			GLsizei GetVertexCount();
			GLsizei GetIndexCount();

			void DeleteBuffers();
			void ClearBuffers();

			GLenum GetDrawType();
			void SetDrawType(GLenum val);

		protected:

			nglBufferObject<nglVertex3P>* vertices;
			nglBufferObject<unsigned short>* indices;
			nglBufferObject<nglVec2>* uvCoords;
			nglBufferObject<nglVertex3P>* normals;
			nglBufferObject<nglRGB3F>* colors;


			GLenum _drawType;

		};

	}


}

#endif // nglRawMesh_h__



