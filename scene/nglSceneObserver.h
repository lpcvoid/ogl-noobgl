#include "nglCommon.h"

#ifndef nglSceneObserver_h__
#define nglSceneObserver_h__


class nglSceneObserver {

public:
	nglSceneObserver();
	virtual ~nglSceneObserver();

	virtual glm::mat4 GetViewMatrix() = 0;
	virtual glm::mat4 GetProjectionMatrix() = 0;
	virtual glm::mat4 GetProjectionViewMatrix() = 0;

	virtual void LookAt(glm::vec3 targetCoords) = 0;

};


#endif // nglSceneObserver_h__