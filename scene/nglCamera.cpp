
#include "nglCamera.h"
#include <iostream>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include <vector>
#include "glm/fwd.hpp"
#include "glm/detail/type_vec.hpp"
#include <glm/gtc/matrix_transform.hpp>
#define _USE_MATH_DEFINES
#include <math.h>



static const float MaxVerticalAngle = 89.0f; //must be less than 90 to avoid gimbal lock

static inline float RadiansToDegrees(float radians) {
	return radians * 180.0f / (float)M_PI;
}

nglCamera::nglCamera():
	_horizontalAngle(0.0f),
	_verticalAngle(0.0f),
	_fieldOfView(50.0f),
	_nearPlane(0.5f),
	_farPlane(1000.0f),
	_viewportAspectRatio(16.0f/9.0f),
	_projection_dirty(true),
	_projection_view_dirty(true),
	_set_projection_manually(false)
{
	this->_lookatType = NGL_CAMERA_LOOKAT_TYPE_COORDS;
	SetNodeType(NGL_SCENENODE_TYPE_CAMERA);

}

nglCamera::~nglCamera()
{

}

// void nglCamera::LookAt(nglSceneNode* pNode)
// {
// 	this->_lookatType = NGL_CAMERA_LOOKAT_TYPE_NODE;
// 	this->_lookatTarget = pNode;
// }

void nglCamera::LookAt(glm::vec3 targetCoords)
{
	this->_lookatType = NGL_CAMERA_LOOKAT_TYPE_COORDS;
	this->_lookatCoordinates = targetCoords;

	glm::vec3 direction = glm::normalize(targetCoords - GetPosition());
	_verticalAngle = RadiansToDegrees(asinf(-direction.y));
	_horizontalAngle = -RadiansToDegrees(atan2f(-direction.x, -direction.z));
	normalizeAngles();
}

glm::mat4 nglCamera::GetProjectionViewMatrix()
{
	if (_projection_view_dirty){
		//TODO seperate both into different dirty flags
		this->_view = this->GetViewMatrix();
		this->_projection = this->GetProjectionMatrix();
		//order is important!
		this->_projectionView = this->_projection * this->_view;
		_projection_view_dirty = false;
	}

	
	return _projectionView;
}

float nglCamera::GetFieldOfView() const
{
	return _fieldOfView;
}

void nglCamera::setFieldOfView(float fieldOfView)
{
	assert(fieldOfView > 0.0f && fieldOfView < 180.0f);
	_fieldOfView = fieldOfView;
}

float nglCamera::GetNearPlane() const
{
	return _nearPlane;
}

float nglCamera::GetFarPlane() const
{
	return _farPlane;
}

void nglCamera::SetNearAndFarPlanes(float nearPlane, float farPlane)
{
	assert(nearPlane > 0.0f);
	assert(farPlane > nearPlane);
	_nearPlane = nearPlane;
	_farPlane = farPlane;
}

glm::mat4 nglCamera::GetViewMatrix()
{
	//also works
	//return glm::lookAt(GetPosition(),GetPosition() + GetForwardVector(),GetUpVector());
	return this->GetOrientation() * glm::translate(glm::mat4(), -this->GetPosition());
}

glm::mat4 nglCamera::GetProjectionMatrix()
{
	if (_projection_dirty)
	{
		if (!_set_projection_manually){
			_projection = glm::perspective(_fieldOfView, _viewportAspectRatio, _nearPlane, _farPlane);
			_projection_dirty = false;
		}
			
	}
	return _projection;
}

glm::mat4 nglCamera::GetOrientation()
{
	glm::mat4 orientation = GetRotation();
	//orientation = glm::rotate(orientation, _verticalAngle, glm::vec3(1,0,0));
	//orientation = glm::rotate(orientation, _horizontalAngle, glm::vec3(0,1,0));
	return orientation;
}



void nglCamera::normalizeAngles()
{
	_horizontalAngle = fmodf(_horizontalAngle, 360.0f);
	//fmodf can return negative values, but this will make them all positive
	if(_horizontalAngle < 0.0f)
		_horizontalAngle += 360.0f;

	if(_verticalAngle > MaxVerticalAngle)
		_verticalAngle = MaxVerticalAngle;
	else if(_verticalAngle < -MaxVerticalAngle)
		_verticalAngle = -MaxVerticalAngle;
}

void nglCamera::OffsetOrientation(float upAngle, float rightAngle)
{
	_horizontalAngle += rightAngle;
	_verticalAngle += upAngle;
	this->Rotate(_horizontalAngle,0,_verticalAngle);
	//normalizeAngles();
}

void nglCamera::OrientateHorizontally(float angle)
{
	this->OffsetOrientation(0,angle);
}

void nglCamera::SetAspectRatio(float ar)
{
	_viewportAspectRatio = ar;
}

void nglCamera::SetProjectionMatrix(glm::mat4& proj_mat)
{
	_projection = proj_mat;
	_projection_dirty = true;
}

void nglCamera::IncrementFieldOfView(float deltaFOV)
{
	_fieldOfView =+ deltaFOV;
	_projection_dirty = true;
}

void nglCamera::SetProjectionManually(bool manually)
{
	_set_projection_manually = manually;
}
