// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "nglGeometry.h"

#ifndef nglLoader_h__
#define nglLoader_h__

class nglLoader {

public:
	virtual ~nglLoader() {};
	virtual nglGeometry* LoadFromFile(std::string& file) = 0;

};

#endif // nglLoader_h__