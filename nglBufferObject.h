// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#ifndef nglBufferObject_h__
#define nglBufferObject_h__

#include "nglObject.h"
#include "nglCommon.h"

enum NGL_BUFFEROBJECT_USAGE {
	/**
	* Mesh data is sent once and very rarely updated.
	*/
	NGL_BUFFEROBJECT_USAGE_STATIC_DRAW = GL_STATIC_DRAW,

	/**
	* Mesh data is updated occasionally (once per frame or less).
	*/
	NGL_BUFFEROBJECT_USAGE_DYNAMIC_DRAW = GL_DYNAMIC_DRAW
};

template <typename T>
class nglBufferObject : public nglObject{

protected:
	std::vector<T> _data;
	
private:
	NGL_ARRAY_DATA_TYPE _type;
	NGL_BUFFEROBJECT_USAGE _usage;
	int _offset;
	int _stride;

public:
	~nglBufferObject();
	nglBufferObject(NGL_ARRAY_DATA_TYPE type);

	//preallocate n elements, NOT BYTES
	void PreAllocate(int element_count);

	//clear buffer contents
	void Clear();

	void AddElement(T element);
	void SetElement(T element, int index);
	void SetBufferData(std::vector<T>& data, NGL_BUFFEROBJECT_USAGE usage = NGL_BUFFEROBJECT_USAGE_STATIC_DRAW);
	void SetOffset(int offset);
	void SetStride(int stride);
	int GetStride();
	int GetOffset();

	void* GetBufferData();
	int GetDataLength();

	int CountElements();

	//Gets an element of type T.
	//index is measured in multiples of sizeof(T)
	//
	//0---1---2---3---4---5---6
	//0123456789... <- bytes
	T GetElement(int index);

	NGL_BUFFEROBJECT_USAGE GetBufferUsage();

	void SetBufferType(NGL_ARRAY_DATA_TYPE type);
	NGL_ARRAY_DATA_TYPE GetBufferType();

};

template <typename T>
void nglBufferObject<T>::Clear()
{
	this->_data.clear();
}

template <typename T>
void nglBufferObject<T>::SetElement(T element, int index)
{
	this->_data[index] = element;
}

template <typename T>
void nglBufferObject<T>::AddElement(T element)
{
	this->_data.push_back(element);
}

template <typename T>
void nglBufferObject<T>::PreAllocate(int element_count)
{
	this->_data.resize(element_count);
}

template <typename T>
NGL_ARRAY_DATA_TYPE nglBufferObject<T>::GetBufferType()
{
	return _type;
}

template <typename T>
void nglBufferObject<T>::SetBufferType(NGL_ARRAY_DATA_TYPE type)
{
	this->_type = type;
}

template <typename T>
NGL_BUFFEROBJECT_USAGE nglBufferObject<T>::GetBufferUsage()
{
	return _usage;
}

template <typename T>
int nglBufferObject<T>::GetDataLength()
{
	return this->_data.size() * sizeof(T);
}

template <typename T>
void* nglBufferObject<T>::GetBufferData()
{
	return this->_data.data();
}

template <typename T>
int nglBufferObject<T>::GetOffset()
{
	return _offset;
}

template <typename T>
int nglBufferObject<T>::GetStride()
{
	return _stride;
}

template <typename T>
void nglBufferObject<T>::SetStride(int stride)
{
	_stride = stride;
}

template <typename T>
void nglBufferObject<T>::SetOffset(int offset)
{
	_offset = offset;
}

template <typename T>
void nglBufferObject<T>::SetBufferData(std::vector<T>& data , NGL_BUFFEROBJECT_USAGE usage)
{
	if (data.size() > this->_data.size())
		this->PreAllocate(data.size());

	for (size_t i = 0; i < data.size();i++)
	{
		this->_data[i] = data[i];
	}

	this->_usage = usage;

}

template <typename T>
nglBufferObject<T>::nglBufferObject(NGL_ARRAY_DATA_TYPE type)
{
	this->_type = type;
	this->_usage = NGL_BUFFEROBJECT_USAGE_STATIC_DRAW;
	this->_offset = 0;
	this->_stride = 0;
}

template <typename T>
nglBufferObject<T>::~nglBufferObject()
{

}

template <typename T>
int nglBufferObject<T>::CountElements()
{
	return this->_data.size();
}

template <typename T>
T nglBufferObject<T>::GetElement(int index)
{
	return this->_data[index];
}


#endif // nglBufferObject_h__