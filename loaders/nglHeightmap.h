// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include "nglCommon.h"
#include <vector>

#ifndef nglHeightmap_h__
#define nglHeightmap_h__

namespace ngl {

	namespace loaders {

		enum HEIGHTMAP_DATA {

			//heightmap provides height data
			HEIGHTMAP_DATA_HEIGHT = 1,
			//heightmap provides color
			HEIGHTMAP_DATA_COLORS = 2

		};

		//Represents base for classes that load height data from somewhere
		class Heightmap {

		public:

			Heightmap();
			virtual ~Heightmap();;
			virtual bool LoadFromFile(std::string file) = 0;

			//only defined if this hmap provides height info - which should generally be always since this is a heightmap.
			virtual float GetHeight(float x, float y, bool check_params = false) = 0;

			//only defined if this hmap provides color info!
			virtual nglRGB3F GetColorValue(int x, int y, bool check_params = false) = 0;
			
			//returns a mask that says what type of data this heightmap can provide.
			int GetProvidedDataMask(); 
			nglVec2 GetSize();

		protected:
			nglVec2 _size;
			//indicates what type of data this heightmap can provide.
			//if color flag is set, then GetColorValue() is defined.
			//if height flag is set, then GetHeight() is defined.
			int _provided_data;


		};

	}

}

#endif // nglHeightmap_h__