#include <iostream>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include <vector>
#include "glm/fwd.hpp"
#include "glm/detail/type_vec.hpp"

#include "nglSceneNode.h"
#include "nglCamera.h"

#ifndef nglScene_h__
#define nglScene_h__

class nglScene{

public:
	nglScene();
	~nglScene();
	void AddSceneNode(nglSceneNode* pNode);
	void AddCamera(nglCamera* cam);
	std::vector<nglSceneNode*> GetChildren();
	template <typename T>
	std::vector<T*> GetChildren(NGL_SCENENODE_TYPE node_type);
	//bool Intersect();

	//return active scene camera
	nglCamera* GetActiveCamera();
	nglCamera* GetCamera(int index);

private:
	nglSceneNode* _sceneRootNode;
	//array of all nodes attached, so we can search for IDs more easy!
	std::vector<nglSceneNode*> _nodes;
	//array of all cameras attached
	std::vector<nglCamera*> _cameras;
	//first camera that was added to scene
	nglCamera* _active_cam;

};

template <typename T>
std::vector<T*>nglScene::GetChildren(NGL_SCENENODE_TYPE node_type)
{
	std::vector<nglSceneNode*> children = this->GetChildren();
	std::vector<T*> type_children;
	for (unsigned int i = 0; i < children.size();i++)
	{
		if (children[i]->GetNodeType() == node_type)
			type_children.push_back((T*)children[i]);

	}

	return type_children;
}

#endif // nglScene_h__