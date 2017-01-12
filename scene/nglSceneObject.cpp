#include "nglSceneObject.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

ngl::scene::SceneObject::SceneObject()
{
	this->ResetTransformation();
	_visable = true;
}

ngl::scene::SceneObject::~SceneObject()
{

}

void ngl::scene::SceneObject::SetPosition(nglVec3& pos)
{
	this->_pos = pos;
	this->SetTransformDirty();
}

void ngl::scene::SceneObject::Move(nglVec3& pos)
{
	//glm::vec3 newPos = *pos + this->_pos;
	this->_pos += pos;
	this->SetTransformDirty();
}

nglVec3 ngl::scene::SceneObject::GetPosition()
{
	return this->_pos;
}

void ngl::scene::SceneObject::SetRotation(nglMat4& rot)
{
	this->_rot = rot;
	this->SetTransformDirty();
}

nglMat4 ngl::scene::SceneObject::GetRotation()
{
	return this->_rot;
}

void ngl::scene::SceneObject::RotateX(float degrees)
{
	this->_rot =  glm::rotate(glm::mat4(), degrees, glm::vec3(1,0,0));
	this->SetTransformDirty();
}

void ngl::scene::SceneObject::RotateY(float degrees)
{
	this->_rot =  glm::rotate(glm::mat4(), degrees, glm::vec3(0,1,0));
	this->SetTransformDirty();
}

void ngl::scene::SceneObject::RotateZ(float degrees)
{
	this->_rot =  glm::rotate(glm::mat4(), degrees, glm::vec3(0,0,1));
	this->SetTransformDirty();
}

void ngl::scene::SceneObject::Rotate(float degrees, nglVec3& axis)
{
	this->_rot =  glm::rotate(glm::mat4(), degrees, axis);
	this->SetTransformDirty();
}

void ngl::scene::SceneObject::Rotate(float heading, float pitch, float roll)
{
	
 	nglMat4 rot_pitch = glm::rotate(glm::mat4(), pitch, glm::vec3(1,0,0));
 	nglMat4 rot_heading = glm::rotate(glm::mat4(), heading, glm::vec3(0,1,0));
 	nglMat4 rot_roll = glm::rotate(glm::mat4(), roll, glm::vec3(0,0,1));

	//_rot = rot_heading * rot_pitch * rot_roll;
	//_rot =  rot_pitch * rot_heading * rot_roll; //close
	//_rot =  rot_pitch * rot_roll * rot_heading;
	//_rot =  rot_roll * rot_pitch * rot_heading;
	_rot =  rot_roll * rot_pitch * rot_heading;

	//_rot = glm::yawPitchRoll(NGL_DEGTORAD(heading),NGL_DEGTORAD(pitch),NGL_DEGTORAD(roll));
	this->SetTransformDirty();
}

void ngl::scene::SceneObject::SetScale(nglVec3& scale)
{
	this->_scale = scale;
	this->SetTransformDirty();
}

void ngl::scene::SceneObject::SetUniformScale(float scale)
{
	this->_scale.x = scale;
	this->_scale.y = scale;
	this->_scale.z = scale;
	this->SetTransformDirty();
}

nglVec3 ngl::scene::SceneObject::GetScale()
{
	return this->_scale;
}

void ngl::scene::SceneObject::ResetTransformation()
{
	this->_pos = glm::vec3(0.0f);
	this->_rot = glm::mat4(1.0);
	this->_scale = glm::vec3(1,1,1);
	this->_cachedTransform = glm::mat4(1.0);
	this->SetTransformDirty();
}

nglMat4 ngl::scene::SceneObject::GetTransformationMatrix()
{
	//scale
	//rotation
	//translation

	//char* bla = (char*)malloc(16 * 4);
	//memcpy(bla,&transformation[0][0],16*4);

	if (this->_transformDirty){
		glm::mat4 transformation, translation, scale;

		translation = glm::translate(glm::mat4(),this->_pos);
		//scale = glm::scale( glm::mat4(),this->_scale);

		transformation = glm::mat4();

		//transformation *= scale;
		transformation *= translation;
		transformation *= _rot;


// 		transformation *= scale;
// 		transformation *= translation;
// 		transformation *= _rot;
		

		this->_cachedTransform = transformation;
		this->_transformDirty = false;
	}

	return this->_cachedTransform;
}



void ngl::scene::SceneObject::SetTransformDirty()
{
	this->_transformDirty = true;
	//signal all attached slots
	OnDirty.emit();
}

void ngl::scene::SceneObject::ToggleVisability()
{
	_visable = !_visable;
}

bool ngl::scene::SceneObject::IsVisable()
{
	return _visable;
}

bool ngl::scene::SceneObject::IsTransformDirty()
{
	return _transformDirty;
}

void ngl::scene::SceneObject::SetVisable(bool vis)
{
	_visable = vis;
}

void ngl::scene::SceneObject::SetHeight(float height)
{
	_pos.y = height;
	this->SetTransformDirty();
}

nglVec3 ngl::scene::SceneObject::GetForwardVector()
{
	glm::vec4 forward = glm::inverse(_rot) * glm::vec4(0,0,-1,1);
	return glm::vec3(forward);
}

nglVec3 ngl::scene::SceneObject::GetRightVector()
{
	glm::vec4 right = glm::inverse(_rot) * glm::vec4(1,0,0,1);
	return glm::vec3(right);
}

nglVec3 ngl::scene::SceneObject::GetUpVector()
{
	glm::vec4 up = glm::inverse(_rot) * glm::vec4(0,1,0,1);
	return glm::vec3(up);
}


