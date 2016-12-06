#pragma once

#include "oglt_util.h"

#include "oglt_callback.h"

namespace oglt {
	void glutBackendInit(int argc, char** argv, int ogltOptions);
	bool glutBackendCreateWindow(uint width, uint height, const char* title, int ogltOptions);
	void glutBackendRun(ICallback* pCallback);
	void glutBackendSetCursor(int x, int y);
	void glutBackendSwapBuffers();
}