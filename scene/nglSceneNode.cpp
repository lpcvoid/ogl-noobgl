#include "nglSceneNode.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <crtdbg.h>



nglSceneNode::nglSceneNode()
{
	this->SetNodeType(NGL_SCENENODE_TYPE_NODE);
	//we want to know when this scene node's local transform changes,
	//so we can set all children's worldtransform to dirty!
	this->OnDirty.connect(this,&nglSceneNode::SetNodeAndChildrenDirty);
	this->ResetTransformation();
}

nglSceneNode::nglSceneNode(std::string name)
{
	this->_name = name;
	this->SetNodeType(NGL_SCENENODE_TYPE_NODE);
	this->OnDirty.connect(this,&nglSceneNode::SetNodeAndChildrenDirty);
	this->ResetTransformation();
}

nglSceneNode::~nglSceneNode()
{
	this->DetachFromParent();
	this->DetachAllChildren();

	//if a camera is watching this object, we MUST tell it not to anymore! Otherwise, 
	//this camera isn't ware that this object doesn't exist anymore.
	//we also tell camera the last position of this node,
	//so it can remain focused there.
	//if (this->_watchingCamera != NULL)
	//	this->_watchingCamera->RemoveWatchedNode(this->_pos);


}

void nglSceneNode::AddChildNode(nglSceneNode* pNode)
{
	this->_children.push_back(pNode);
	pNode->SetParent(this);
}

void nglSceneNode::SetNodeName(std::string name)
{
	this->_name = name;
}

std::string nglSceneNode::GetNodeName()
{
	return this->_name;
}

std::vector<nglSceneNode*> nglSceneNode::GetChildren()
{
	std::vector<nglSceneNode*> childList;

	this->GetChildren(childList);

	return childList;
}

void nglSceneNode::GetChildren(std::vector<nglSceneNode*>& childList)
{

	if (this->CountDirectChildren() > 0){

		int count = this->CountDirectChildren();
		for (int i = 0;i < count;i++)
		{
			if (this->_children[i]->IsVisable())
			{
				childList.push_back(this->_children[i]);
				this->_children[i]->GetChildren(childList);
			}

		}

	}

}

int nglSceneNode::CountChildren()
{

	std::vector<nglSceneNode*> childList = this->GetChildren();
	int size = childList.size();
	return (size);

}

nglSceneNode* nglSceneNode::GetParent()
{
	return this->_parent;
}

void nglSceneNode::DetachFromParent()
{
	if (this->HasParent()){
		this->_parent->RemoveChild(this);
	}
}

int nglSceneNode::CountDirectChildren()
{
	return this->_children.size();
}


void nglSceneNode::RemoveChild(nglSceneNode* pNode)
{
	this->_children.erase(std::remove(this->_children.begin(), this->_children.end(), pNode), this->_children.end());
}

bool nglSceneNode::HasParent()
{
	return (this->_parent != NULL);
}

void nglSceneNode::SetParent(nglSceneNode* pNode)
{
	this->_parent = pNode;
}

NGL_SCENENODE_TYPE nglSceneNode::GetNodeType()
{
	return this->_type;
}

void nglSceneNode::SetNodeType(NGL_SCENENODE_TYPE nodeType)
{
	this->_type = nodeType;
}

void nglSceneNode::DetachAllChildren()
{

	for (int i = 0;i < this->_children.size();i++)
	{
		this->_children[i]->DetachFromParent();
	}

	this->_children.clear();
}


void nglSceneNode::SetNodeAndChildrenDirty()
{

	//we need to set all child nodes, and this one, to dirty.
	//this means, that all cached world transforms become invalid, and must be recalculated next render pass.
	//each node inherits a sceneobject. It also contains a transform (the nodes local transform).
	//that isn't touched, since other nodes do not care about that.
	_cachedWorldDirty = true;

	//now we need to set all child nodes dirty.
	std::vector<nglSceneNode*> children = this->GetChildren();
	for (int i = 0; i< children.size();i++)
	{
		children[i]->SetWorldDirty();
	}
}

std::string nglSceneNode::ToString()
{
	return std::string("nglSceneNode(), name=" + this->_name + ",node_type=" + std::to_string((long long) this->_type));
}

void nglSceneNode::SetWorldDirty()
{
	_cachedWorldDirty = true;
}

nglMat4 nglSceneNode::GetWorldTransform()
{
	if (_cachedWorldDirty){

		//recalculate world transform from first non-dirty node in hierachy.
		//root node is always clean.

		//first, find first non-dirty parent matrix somewhere in heirachy.
		bool found_clean_parent = false;
		nglSceneNode* parent = _parent;
		while (!found_clean_parent)
		{
			if (parent->IsWorldDirty())
				parent = parent->GetParent();
			else {
				found_clean_parent = true;

			}
		}

		//now let all children of this clean parent recursively recalculate world transforms!
		parent->ChildrenRecalculateWorldTransform();

	}

	return _cachedWorld;
}

void nglSceneNode::RecalculateWorldTransformFromParent()
{

	//only do this if world matrix is in any way dirty.
	if (_cachedWorldDirty){

		nglMat4 parent_world;
		if (_parent)
			parent_world = _parent->GetWorldTransform();
		else {
			parent_world = nglMat4(); //identity
		};

		_cachedWorld = parent_world * this->GetTransformationMatrix();
		_cachedWorldDirty = false;

// 		SetPosition(_parent->GetPosition() + GetPosition());
// 		SetRotation(_parent->GetRotation() * GetRotation());
// 		SetScale(_parent->GetScale() + GetScale());


	}

}

bool nglSceneNode::IsWorldDirty()
{
	return _cachedWorldDirty;
}

void nglSceneNode::ChildrenRecalculateWorldTransform()
{
	if (this->CountDirectChildren() > 0){

		int count = this->CountDirectChildren();

		for (int i = 0;i < count;i++)
		{
			this->_children[i]->RecalculateWorldTransformFromParent();
			this->_children[i]->ChildrenRecalculateWorldTransform();
		}

	}
}

void nglSceneNode::SetClean()
{
	_cachedWorldDirty = false;
}


nglVec3 nglSceneNode::GetTransformedPosition()
{
	nglVec3 trans_pos = nglVec3(nglVec4(GetPosition(),1.0f) * GetWorldTransform());
	return trans_pos;
}