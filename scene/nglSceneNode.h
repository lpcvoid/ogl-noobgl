#pragma once

#ifndef nglSceneNode_h__
#define nglSceneNode_h__

#include <iostream>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include <vector>
#include "glm/fwd.hpp"
#include "glm/detail/type_vec.hpp"
#include "nglSceneObject.h"
#include "sigslot.h"

//#include "nglCamera.h"

enum NGL_SCENENODE_TYPE {

	NGL_SCENENODE_TYPE_NODE = 100,
	NGL_SCENENODE_TYPE_GEOMETRY = 101,
	NGL_SCENENODE_TYPE_TERRAIN = 102,
	NGL_SCENENODE_TYPE_RENDERABLECONTAINER = 103,
	NGL_SCENENODE_TYPE_LIGHT = 104,
	NGL_SCENENODE_TYPE_CAMERA = 105,
};

//Doesn't inherit nglObject, since it maintains its own dirty state.

class nglSceneNode : public ngl::scene::SceneObject, public sigslot::has_slots<> {

public:
	nglSceneNode();
	nglSceneNode(std::string name);
	virtual ~nglSceneNode();

	NGL_SCENENODE_TYPE GetNodeType();
	void SetNodeType(NGL_SCENENODE_TYPE nodeType);

	void AddChildNode(nglSceneNode* pNode);
	void SetNodeName(std::string name);
	std::string GetNodeName();

	std::vector<nglSceneNode*> GetChildren();

	nglVec3 GetTransformedPosition();


	int CountChildren();
	int CountDirectChildren();
	nglSceneNode* GetParent();
	void DetachFromParent();
	void DetachAllChildren();
	bool HasParent();
	void SetParent(nglSceneNode* pNode);

	void RemoveChild(nglSceneNode* pNode);


	//void NotifyBeingWatched(nglCamera* cam);

	std::string ToString();

	nglMat4 GetWorldTransform();

	//Sets the world matrix of this and all child nodes to dirty, recursively.
	//This is only used internally.
	void SetNodeAndChildrenDirty();
	void SetWorldDirty();
	bool IsWorldDirty();

	//only used for root node.
	void SetClean();

private:

	//************************************
	// This is the same as the other GetChildren() method,
	// but it puts the children into the passed list,
	// instead of returning it.
	// edit : worked, massive speed increase (halfed time :))
	//************************************
	void GetChildren(std::vector<nglSceneNode*>& childList);

	//multiplies nodes local transform (sceneobject) by the parents world transform.
	//this results in this nodes new world transform.
	void RecalculateWorldTransformFromParent();

	//performs RecalculateWorldTransformFromParent() on all children recursively.
	//the caller node does not recalculate her transform.
	//this essentially rebuilds all (if called from root node), or a part of the scene graph in regard to transformations.
	void ChildrenRecalculateWorldTransform();

	std::vector<nglSceneNode*> _children;
	std::string _name;
	nglSceneNode* _parent;
	NGL_SCENENODE_TYPE _type;
	//this is the cached world transform that gets calculated from root node to this node.
	//it can get invalidated if any parent node is changed.
	//if nothing gets changed, this is a cpu-friendly way to save scene node transform up to this point in graph.
	glm::mat4 _cachedWorld;
	bool _cachedWorldDirty;


};

#endif // nglSceneNode_h__