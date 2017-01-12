#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "nglLoader.h"

#ifndef nglLoaderNGF_h__
#define nglLoaderNGF_h__


#define NGLG_VERSION 1

class nglLoaderNGF : public nglLoader {

public:
	nglLoaderNGF();
	virtual ~nglLoaderNGF();

	virtual nglGeometry* LoadFromFile(std::string& file);


};

#endif // nglLoaderNGF_h__