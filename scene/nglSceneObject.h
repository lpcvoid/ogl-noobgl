#pragma once

#ifndef nglSceneObject_h__
#define nglSceneObject_h__

#include "glm/glm.hpp"
#include "nglCommon.h"
#include "../nglCallbacks.h"
#include <vector>
#include "sigslot.h"

/*

This class represents a base class for any object that needs a transform within the scene.

*/

namespace ngl {

	namespace scene {

		class SceneObject {

		public:
			SceneObject();
			virtual ~SceneObject();

			void SetPosition(nglVec3& pos);
			void Move(nglVec3& pos);
			nglVec3 GetPosition();
			void SetHeight(float height);
			

			void SetRotation(nglMat4& rot);
			nglMat4 GetRotation(); 
			void RotateX(float degrees);
			void RotateY(float degrees);
			void RotateZ(float degrees);
			void Rotate(float degrees, nglVec3& axis);
			void Rotate(float heading, float pitch, float roll);

			nglVec3 GetForwardVector();
			nglVec3 GetRightVector();
			nglVec3 GetUpVector();

			void SetScale(nglVec3& scale);
			void SetUniformScale(float scale);
			nglVec3 GetScale();

			void ResetTransformation();
			nglMat4 GetTransformationMatrix();

			//properties
			void ToggleVisability();
			bool IsVisable();
			void SetVisable(bool vis);

			bool IsTransformDirty();

			//Signals
			sigslot::signal0<> OnDirty;
			

		protected:
			void SetObjectTransformDirty();

		private:

			void SetTransformDirty();

			glm::vec3 _pos;
			glm::mat4 _rot;
			glm::vec3 _scale;
			glm::mat4 _cachedTransform;
			bool _transformDirty;

			//scene object properties
			bool _visable;
		};


	}

}


#endif // nglSceneObject_h__