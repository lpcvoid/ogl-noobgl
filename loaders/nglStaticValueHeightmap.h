#include <stdio.h>
#include <stdlib.h>
#include "nglCommon.h"
#include <string>
#include "nglHeightmap.h"

#ifndef nglStaticValueHeightmap_h__
#define nglStaticValueHeightmap_h__

namespace ngl {

	namespace loaders {

		class StaticValueHeightmap : public Heightmap {

		public:
			StaticValueHeightmap();
			virtual ~StaticValueHeightmap();

			void SetStaticValue(float val);
			float GetStaticValue();

			virtual bool LoadFromFile(std::string file);
			virtual float GetHeight(float x, float y, bool check_params = false);
			virtual nglRGB3F GetColorValue(int x, int y, bool check_params = false);


		private:
			float _static_value;
		};

	}

}


#endif // nglStaticValueHeightmap_h__