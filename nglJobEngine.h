#include <stdio.h>
#include <queue>
#include "nglJob.h"
#include <vector>

#ifndef nglJobEngine_h__
#define nglJobEngine_h__

namespace ngl {

	namespace jobs {

		class nglJobEngine {
		public:
			nglJobEngine();
			~nglJobEngine();
			void SetMaxJobs();
			int CountJobs();
			void SetThreadPoolSize(int tps);

		private:
			std::queue<ngl::jobs::nglJob> _jobs;
			//std::vector<std::th>

		};

	}



}

#endif // nglJobEngine_h__