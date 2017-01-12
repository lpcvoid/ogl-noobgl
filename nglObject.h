#include <iostream>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include <vector>
#include "glm/fwd.hpp"
#include "glm/detail/type_vec.hpp"

#ifndef nglObject_h__
#define nglObject_h__

/************************************************************************/
/* Base class for all ngl objects which have state                      */
/************************************************************************/

class nglObject {

public:

	//so we can stuff this into a map
	bool operator<( const nglObject& other) const;

	nglObject();
	virtual ~nglObject();
	virtual void SetDirty(bool state);
	virtual bool IsDirty();

	//flag this object as junk, which can be deleted on next cleanup round
	void FlagToDestroy();
	//check if this object can be deleted
	bool CanBeDestroyed();

	std::string GetName();
	void SetName(std::string& name);

	long GetObjectId() const;

protected:
	volatile bool _dirty;
	volatile int _ref_count;

	std::string _name;

	bool _can_be_destroyed;

	//a unique object id. Has nothing to do with opengl.
	//unique across complete address space
	//not unique across computers
	volatile long _objectId;


};


#endif // nglObject_h__