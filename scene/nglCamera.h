#include <iostream>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include <vector>
#include "glm/fwd.hpp"
#include "glm/detail/type_vec.hpp"
#include "nglObject.h"
#include "nglSceneObserver.h"
#include "nglSceneNode.h"

//#include "nglSceneNode.h"

#ifndef nglCamera_h__
#define nglCamera_h__



enum NGL_CAMERA_LOOKAT_TYPE {
	NGL_CAMERA_LOOKAT_TYPE_NODE,
	NGL_CAMERA_LOOKAT_TYPE_COORDS
};

class nglCamera : public nglSceneNode , public nglSceneObserver{

public:
	nglCamera();
	~nglCamera();

	//void LookAt(nglSceneNode* pNode);
	void LookAt(glm::vec3 targetCoords);


	/**
	The vertical viewing angle of the camera, in degrees.

	Determines how "wide" the view of the camera is. Large angles appear to be zoomed out,
	as the camera has a wide view. Small values appear to be zoomed in, as the camera has a
	very narrow view.

	The value must be between 0 and 180.
	*/
	float GetFieldOfView() const;
	void setFieldOfView(float fieldOfView);
	void IncrementFieldOfView(float deltaFOV);

	/**
	The closest visible distance from the camera.
	Objects that are closer to the camera than the near plane distance will not be visible.
	Value must be greater than 0.
	*/
	float GetNearPlane() const;

	/**
	The farthest visible distance from the camera.
	Objects that are further away from the than the far plane distance will not be visible.
	Value must be greater than the near plane
	*/
	float GetFarPlane() const;

	/**
	Sets the near and far plane distances.

	Everything between the near plane and the var plane will be visible. Everything closer
	than the near plane, or farther than the far plane, will not be visible.

	@param nearPlane  Minimum visible distance from camera. Must be > 0
	@param farPlane   Maximum visible distance from camera. Must be > nearPlane
	*/
	void SetNearAndFarPlanes(float nearPlane, float farPlane);

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetProjectionViewMatrix();
	void SetProjectionMatrix(glm::mat4& proj_mat);

	//If true, camera will use projection matrix provided by SetProjectionMatrix(), otherwise it will calculate on set values.
	void SetProjectionManually(bool manually);

	//void SetHorizontalAngle(float deg);
	//void SetVerticalAngle(float deg);

	/**
	A rotation matrix that determines the direction the camera is looking.
	Does not include translation (the camera's position).
	*/
	glm::mat4 GetOrientation();


	/**
	Offsets the cameras orientation.
	The verticle angle is constrained between 85deg and -85deg to avoid gimbal lock.

	@param upAngle     the angle (in degrees) to offset upwards. Negative values are downwards.
	@param rightAngle  the angle (in degrees) to offset rightwards. Negative values are leftwards.
	*/
	void OffsetOrientation(float upAngle, float rightAngle);
	void OrientateHorizontally(float angle);





	//stops camera from watching a certain Node.
	//void RemoveWatchedNode();
	//same as above, but now you can tell camera last known position
	//it then uses this as lookat, and stays at the place
	//void RemoveWatchedNode(glm::vec3 lastPosition);


	void SetAspectRatio(float ar);

private:
	glm::mat4 _projection;
	glm::mat4 _view;
	glm::mat4 _projectionView;
	float _horizontalAngle;
	float _verticalAngle;
	float _fieldOfView;
	float _nearPlane;
	float _farPlane;
	float _viewportAspectRatio;

	bool _projection_dirty;
	bool _projection_view_dirty;
	bool _set_projection_manually;
	
	//nglSceneNode* _lookatTarget;
	glm::vec3 _lookatCoordinates;
	NGL_CAMERA_LOOKAT_TYPE _lookatType;

	void normalizeAngles();

};


#endif // nglCamera_h__