#include "oglt_app.h"

#include "glut_backend.h"

#include "render_scene.h"

#include <glut.h>
#include <glm\gtc\matrix_transform.hpp>

using namespace oglt;

bool IApp::keyStates[256] = { false };
Cursor IApp::cursor;

OgltApp::OgltApp() {
	
}

OgltApp::~OgltApp() {
	scene::releaseScene(this);
}

void OgltApp::init(int argc, char** argv, int ogltOptions) {
	IApp::init();
	glutBackendInit(argc, argv, ogltOptions);
}

bool OgltApp::createWindow(uint width, uint height, const char* title, int ogltOptions) {
	return glutBackendCreateWindow(width, height, title, ogltOptions);
}

void OgltApp::run() {
	scene::initScene(this);
	glutBackendRun(this);
}

void OgltApp::renderScene() {
	updateTimer();
	scene::renderScene(this);
}

void OgltApp::keyboard(OGLT_KEY key, OGLT_KEY_STATE state){
	switch (state) {
	case OGLT_KEY_PRESS:
		keyStates[key] = true;
		if (key == 'q') {
			scene::releaseScene(this);
			exit(0);
		}
		break;
	case OGLT_KEY_RELEASE:
		keyStates[key] = false;
		break;
	}
}

void OgltApp::mouse(OGLT_BUTTON button, OGLT_BUTTON_STATE state, int x, int y) {
	
}

void OgltApp::mouseMotion(int x, int y) {
	cursor.x = x;
	cursor.y = y;
}

void OgltApp::reshape(uint width, uint height)
{
	viewportWidth = width;
	viewportHeight = height;
	glViewport(0, 0, width, height);
	proj = glm::perspective(45.0f, (float)width / (float)height, 1.0f, 5000.0f);
	orth = glm::ortho(0.0f, (float)width, 0.0f, (float)height);
}

void oglt::OgltApp::swapBuffers()
{
	glutBackendSwapBuffers();
}

void OgltApp::setCursor(int x, int y)
{
	cursor.x = x;
	cursor.y = y;
	glutBackendSetCursor(x, y);
}

void OgltApp::getCursor(int & x, int & y)
{
	x = cursor.x;
	y = cursor.y;
}

bool OgltApp::key(int ikey)
{
	return keyStates[ikey];
}
