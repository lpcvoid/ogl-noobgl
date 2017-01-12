#include "nglRenderQueue.h"




ngl::render::RenderQueue::RenderQueue()
{

}

ngl::render::RenderQueue::~RenderQueue()
{

}

int ngl::render::RenderQueue::GetListCount()
{
	return _lists.size();
}

ngl::render::RenderQueueList* ngl::render::RenderQueue::GetList(int i)
{
	return _lists[i];
}


void ngl::render::RenderQueue::Clear()
{
	int lcount = GetListCount();
	for (unsigned int i = 0; i < lcount;i++)
	{
		delete _lists[i];
	}
	_lists.clear();
}

ngl::render::RenderQueueList* ngl::render::RenderQueue::FindList(nglMaterial* mat)
{
	int lcount = GetListCount();
	for (unsigned int i = 0; i < lcount;i++)
	{
		if (mat == _lists[i]->mat)
		{
			return _lists[i];
		}
			
	}
	return NULL;
}

void ngl::render::RenderQueue::AddContainer(nglRenderableContainer* container)
{

	auto meshes = container->GetMeshes();
	for (unsigned int i = 0; i < meshes.size();i++)
	{

		//check if we want to add meshes to renderlist wihc are flagged as invisible
		if (!meshes[i]->IsVisable())
			continue;

		nglMaterial* mesh_mat = meshes[i]->GetMaterial();

		if (mesh_mat == NULL)
			mesh_mat = container->GetGlobalMaterial();

		if (mesh_mat != NULL)
		{
			//now, check if we already have a list which handles meshes with this material.
			RenderQueueList* mesh_list = NULL;
			if (!(mesh_list = FindList(mesh_mat))){
				mesh_list = new RenderQueueList;
				mesh_list->mat = mesh_mat;
				_lists.push_back(mesh_list);
			}

			RenderQueueEntryMesh mesh_entry;
			mesh_entry.local_transform = meshes[i]->GetTransformationMatrix();
			mesh_entry.world_transform = container->GetWorldTransform();
			mesh_entry.mesh = meshes[i];

			mesh_list->meshes.push_back(mesh_entry);

		}
	}

}

void ngl::render::RenderQueueList::AddBoundingVolumeMeshesFromContainer(nglRenderableContainer* cont)
{
	auto meshes = cont->GetMeshes();
	for (unsigned int i = 0; i < meshes.size();i++)
	{
		RenderQueueEntryMesh mesh_entry;
		mesh_entry.local_transform = meshes[i]->GetTransformationMatrix();
		mesh_entry.world_transform = cont->GetWorldTransform();
		mesh_entry.mesh = meshes[i]->GetBoundingVolume()->GetRenderVolume();

		this->meshes.push_back(mesh_entry);
	}
}

void ngl::render::RenderQueue::AddList(RenderQueueList* list)
{
	_lists.push_back(list);
}

ngl::render::RenderQueueList::~RenderQueueList()
{
}



