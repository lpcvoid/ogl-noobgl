#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "nglGeometry.h"
#include "nglLoader.h"

// Include AssImp
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#ifndef nglAssImpLoader_h__
#define nglAssImpLoader_h__

class AssImpLoader : public nglLoader{

public:

	virtual ~AssImpLoader();
	AssImpLoader();
	virtual nglGeometry* LoadFromFile(std::string& file);

};

#endif // nglAssImpLoader_h__