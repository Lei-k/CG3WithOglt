#pragma once

#include "oglt_util.h"
#include "std_util.h"
#include <glm\glm.hpp>

namespace oglt {
	struct Cursor {
		int x, y;
	};

	class IApp {
	public:
		virtual void getViewport(uint& width, uint& height) { 
			width = this->viewportWidth;
		    height = this->viewportHeight;
		};

		virtual glm::mat4* getProj() { return &proj; }
		virtual glm::mat4* getOrth() { return &orth; }
		virtual float getDeltaTime() { return deltaTime; }
		virtual int getFps() { return fps; }

		virtual void swapBuffers(){}

		virtual void setCursor(int x, int y){}
		virtual void getCursor(int& x, int& y){}

		virtual bool key(int ikey) { return false; }
		virtual bool oneKey(int ikey) { return false; }

	protected:
		virtual void init() {
			deltaTime = 0;
			curFps = 0;
			fps = 0;
			lastClock = clock();
			clockInSecond = 0;
		}

		virtual void updateTimer(){
			clock_t curClock = clock();
			deltaTime = (float)(curClock - lastClock) / (float)CLOCKS_PER_SEC;
			clockInSecond += (curClock - lastClock);
			if (clockInSecond >= CLOCKS_PER_SEC) {
				clockInSecond -= CLOCKS_PER_SEC;
				fps = curFps;
				curFps = 0;
			}
			curFps++;
			lastClock = curClock;
		}

		uint viewportWidth, viewportHeight;
		glm::mat4 proj, orth;

		float deltaTime;
		int fps, curFps;

		clock_t lastClock;
		clock_t clockInSecond;

		static bool keyStates[256];
		static bool oneKeyStats[256];
		static Cursor cursor;
	};
}