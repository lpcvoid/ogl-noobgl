#include <Windows.h>
#include "nglWindow.h"
#include "nglRendererBase.h"
#include "nglScene.h"
#include "nglResourceManager.h"

#ifndef nglEngine_h__
#define nglEngine_h__


struct nglEngine {

	nglWindow* window;
	nglRendererBase* renderer;
	ngl::ResourceManager* resman;
	nglScene* scene;
};

namespace ngl {

	class Engine {

	public:
		Engine();
		~Engine();

		


	};

}


#endif // nglEngine_h__