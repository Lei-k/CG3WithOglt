#pragma once

#include "oglt_iapp.h"

namespace oglt {
	namespace scene {
		void initScene(IApp* app), updateScene(IApp* app), renderScene(IApp* app), releaseScene(IApp* app);
	}
}