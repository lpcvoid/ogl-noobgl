#include <stdio.h>
#include <stdlib.h>
#include "nglCommon.h"
#include <string>
#include "nglHeightmap.h"

#ifndef nglAionHeightmap_h__
#define nglAionHeightmap_h__




namespace ngl {

	namespace loaders {

		struct AionHeightEntry {
			unsigned short height;
			char unknown;
		};

		class AionHeightmap : public Heightmap {

		public:

			virtual bool LoadFromFile(std::string file);

			virtual float GetHeight(float x, float y, bool check_params = false);

		private:
			std::vector<unsigned short> _hmap;

			unsigned short GetElement(int indx, bool safe = false);

		};

	}

}

#endif // nglAionHeightmap_h__