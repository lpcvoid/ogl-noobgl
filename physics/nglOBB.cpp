#include "nglOBB.h"
#include "..\scene\nglRawMesh.h"
#include "..\nglCommon.h"
#include "..\nglBufferObject.h"




bool ngl::physics::OBB::UpdateBoundingVolume(nglBufferObject<nglVertex3P>* vertices, nglBufferObject<unsigned short>* indices)
{

	int vertex_count = vertices->CountElements();

	glm::vec3 temp_compare_distance_pos;
	glm::vec3 temp_compare_distance_neg;
	glm::vec3 temp_center(0,0,0);

	nglVertex3P temp_vertex;

	if (vertex_count > 0){
		temp_vertex = vertices->GetElement(0);
		temp_compare_distance_pos = temp_compare_distance_neg = temp_vertex;
	} else {

		temp_compare_distance_pos = temp_compare_distance_neg = nglVec3(0,0,0);

	}


	for (size_t i = 0; i < vertex_count; i++)
	{
		
		temp_vertex = vertices->GetElement(i);

		//now we compare distances of each vertex, for the positive side!
		if (temp_vertex.x > temp_compare_distance_pos.x)
			temp_compare_distance_pos.x = (temp_vertex.x);
		if (temp_vertex.y > temp_compare_distance_pos.y)
			temp_compare_distance_pos.y = (temp_vertex.y);
		if (temp_vertex.z > temp_compare_distance_pos.z)
			temp_compare_distance_pos.z = (temp_vertex.z);

		//now the same, but we check for the lowest values!
		if (temp_vertex.x < temp_compare_distance_neg.x)
			temp_compare_distance_neg.x = (temp_vertex.x);
		if (temp_vertex.y < temp_compare_distance_neg.y)
			temp_compare_distance_neg.y = (temp_vertex.y);
		if (temp_vertex.z < temp_compare_distance_neg.z)
			temp_compare_distance_neg.z = (temp_vertex.z);

	}

// 	this->_center.x = (temp_compare_distance_pos.x + temp_compare_distance_neg.x) / 2;
// 	this->_center.y = (temp_compare_distance_pos.y + temp_compare_distance_neg.y) / 2;
// 	this->_center.z = (temp_compare_distance_pos.z + temp_compare_distance_neg.z) / 2;

	this->_posHalfwidth = temp_compare_distance_pos;
	this->_negHalfwidth = temp_compare_distance_neg;

	
	this->CalculateRenderVolume();

	//render volume has to be updated the next time it is requested!
	this->SetDirty(true);

	return true;

}

bool ngl::physics::OBB::Collide(BoundingVolume* otherbv, glm::mat4& worldmat, BVCollisionResult* result)
{
	throw std::logic_error("The method or operation is not implemented.");
}

ngl::physics::OBB::OBB()
{
	this->SetDirty(true);
	_negHalfwidth = glm::vec3(1,1,1);
	_posHalfwidth = glm::vec3(1,1,1);
}


void ngl::physics::OBB::CalculateRenderVolume()
{

	

	nglBufferObject<nglVertex3P>* vb;
	nglBufferObject<unsigned short>* ib;

	vb = this->_cachedMeshinfo->GetBufferVertices();
	ib = this->_cachedMeshinfo->GetBufferIndices();

	if (vb && ib) {
		this->_cachedMeshinfo->ClearBuffers();
	}else {
		vb = new nglBufferObject<nglVertex3P>(NGL_ARRAY_DATA_TYPE_VERTEX);
		ib = new nglBufferObject<unsigned short>(NGL_ARRAY_DATA_TYPE_INDEX);
		this->_cachedMeshinfo->SetBufferIndices(ib);
		this->_cachedMeshinfo->SetBufferVertices(vb);
	}

// 	vb->PreAllocate(20);
// 	ib->PreAllocate(20);

	nglVertex3P v;
	glm::vec3 center = glm::vec3(0,0,0); //GetCenter();
	//upper quad
	v.x = center.x + _negHalfwidth.x;
	v.y = center.y + _posHalfwidth.y;
	v.z = center.z + _posHalfwidth.z;
	vb->AddElement(v);
	ib->AddElement(0);

	v.x = center.x + _posHalfwidth.x;
	v.y = center.y + _posHalfwidth.y;
	v.z = center.z + _posHalfwidth.z;
	vb->AddElement(v);
	ib->AddElement(1);

	v.x = center.x + _posHalfwidth.x;
	v.y = center.y + _posHalfwidth.y;
	v.z = center.z + _negHalfwidth.z;
	vb->AddElement(v);
	ib->AddElement(2);

	v.x = center.x + _negHalfwidth.x;
	v.y = center.y + _posHalfwidth.y;
	v.z = center.z + _negHalfwidth.z;
	vb->AddElement(v);
	ib->AddElement(3);
	
	ib->AddElement(0);

	// lower quad

	v.x = center.x + _negHalfwidth.x;
	v.y = center.y + _negHalfwidth.y;
	v.z = center.z + _posHalfwidth.z;
	vb->AddElement(v);
	ib->AddElement(4);

	v.x = center.x + _posHalfwidth.x;
	v.y = center.y + _negHalfwidth.y;
	v.z = center.z + _posHalfwidth.z;
	vb->AddElement(v);
	ib->AddElement(5);

	v.x = center.x + _posHalfwidth.x;
	v.y = center.y + _negHalfwidth.y;
	v.z = center.z + _negHalfwidth.z;
	vb->AddElement(v);
	ib->AddElement(6);

	v.x = center.x + _negHalfwidth.x;
	v.y = center.y + _negHalfwidth.y;
	v.z = center.z + _negHalfwidth.z;
	vb->AddElement(v);
	ib->AddElement(7);
	
	ib->AddElement(4);
	ib->AddElement(5);
	ib->AddElement(1);
	ib->AddElement(2);
	ib->AddElement(6);
	ib->AddElement(7);
	ib->AddElement(3);

	this->SetDirty(false);
}

ngl::scene::RawMesh* ngl::physics::OBB::GetRenderVolume()
{
	if (this->IsDirty())
		CalculateRenderVolume();
	return _cachedMeshinfo;
}

glm::vec3 ngl::physics::OBB::GetPositiveHalfWidth()
{
	return _posHalfwidth;
}

glm::vec3 ngl::physics::OBB::GetNegativeHalfWidth()
{
	return _negHalfwidth;
}

bool ngl::physics::OBB::Intersect(Ray& ray, glm::mat4& world_transform)
{

	float maxS = 1000000.0f;
	float minT = 0.0f;

	glm::vec3 OBBposition_worldspace(world_transform[3].x, world_transform[3].y, world_transform[3].z);

	// 	obb_center_worldspace.x = temp_obb_world.x;
	// 	obb_center_worldspace.y = temp_obb_world.y;
	// 	obb_center_worldspace.z = temp_obb_world.z;

	//glm::vec3 diff = obb_center_worldspace - ray.start;
	glm::vec3 diff = OBBposition_worldspace - ray.start;

	auto pos_hw = GetPositiveHalfWidth();
	auto neg_hw = GetNegativeHalfWidth();

	for (unsigned int i = 0; i < 3; i++)
	{
		glm::vec3 axis(world_transform[i].x, world_transform[i].y, world_transform[i].z);



		float e = glm::dot(axis,diff);
		float f = glm::dot(ray.direction,axis);

		//standard case
		if (fabs(f) > 0.001f){

			float t1 = (e + neg_hw[i]) / f;
			float t2 = (e + pos_hw[i]) / f;

			// fix order
			// We want t1 to represent the nearest intersection,
			// so if it's not the case, invert t1 and t2

			if (t1 > t2){

				float temp = t1;
				t1 = t2;
				t2 = temp;
			}

			// tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
			// adjust min and max values
			if (t1 > minT)
				minT = t1;
			if (t2 < maxS)
				maxS = t2;

			// ray passes by box?
			if (maxS < minT)
				return false;
		} else {
			// Rare case : the ray is almost parallel to the planes, so they don't have any "intersection"
			if(-e+neg_hw[i] > 0.0f || -e+pos_hw[i] < 0.0f)
				return false;

		}


	}

	return true;
}

