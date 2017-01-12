#include "nglScene.h"
#include "nglMesh.h"
#include "nglGeometry.h"
#include "nglSceneNode.h"




nglScene::nglScene()
{
	this->_sceneRootNode = new nglSceneNode(std::string("root_node"));
	this->_sceneRootNode->SetClean();
	_active_cam = NULL;
}

nglScene::~nglScene()
{
	delete this->_sceneRootNode;
}

void nglScene::AddSceneNode(nglSceneNode* pNode)
{
	this->_sceneRootNode->AddChildNode(pNode);
	_nodes.push_back(pNode);
}

std::vector<nglSceneNode*> nglScene::GetChildren()
{
	return this->_sceneRootNode->GetChildren();
}

void nglScene::AddCamera(nglCamera* cam)
{
	this->_cameras.push_back(cam);
	if (_active_cam == NULL)
		this->_active_cam = cam;
}

nglCamera* nglScene::GetActiveCamera()
{
	return _active_cam;
}

nglCamera* nglScene::GetCamera(int index)
{
	if (index >= 0 && index < _cameras.size())
		return _cameras[index];
}




