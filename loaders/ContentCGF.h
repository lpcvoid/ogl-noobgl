#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <memory>
#include "nglCGFDefs.h"

#ifndef ContentCGF_h__
#define ContentCGF_h__
namespace ngl {

	namespace loaders {

		namespace cgf {

			class ContentCGF {

			public:
				ContentCGF(std::string& name);
				void AddNode( NodeCGF* pNode );
				int GetNodeCount();
				NodeCGF* GetNode( int i );

				void AddMaterial( MaterialCGF* pNode );
				int GetMaterialCount();
				MaterialCGF* GetMaterial( int i );

			private:
				std::string _filename;
				std::vector<NodeCGF*> _nodes;
				std::vector<MaterialCGF*> _materials;

			};

		}
	}
}

#endif // ContentCGF_h__

