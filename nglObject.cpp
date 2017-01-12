#include "nglObject.h"
#include <Windows.h>

static volatile long GLOBAL_OBJ_ID_COUNTER = 0;


nglObject::nglObject()
{
	this->_dirty = false;
	_can_be_destroyed = false;
	InterlockedIncrement(&GLOBAL_OBJ_ID_COUNTER);
	_objectId = InterlockedExchangeAdd(&GLOBAL_OBJ_ID_COUNTER, 0);
}

void nglObject::SetDirty(bool state)
{
	this->_dirty = state;
}

bool nglObject::IsDirty()
{
	return _dirty;
}

nglObject::~nglObject()
{

}

void nglObject::FlagToDestroy()
{
	_can_be_destroyed = true;
}

bool nglObject::CanBeDestroyed()
{
	return _can_be_destroyed;
}

long nglObject::GetObjectId() const
{
	return _objectId;
}

std::string nglObject::GetName()
{
	return _name;
}

void nglObject::SetName(std::string& name)
{
	_name = name;
}

bool nglObject::operator<(const nglObject& other) const
{
	int ownId,otherId;
	ownId = GetObjectId();
	otherId = other.GetObjectId();
	return (ownId < otherId);
}
