#include "glut_backend.h"

#include <GL\glew.h>

#include <glut.h>

#include "std_util.h"

using namespace oglt;

static ICallback* spCallback = NULL;
static int sOgltOptions = 0;

void initCallback();

void oglt::glutBackendInit(int argc, char** argv, int ogltOptions) {
	glutInit(&argc, argv);

	uint DisplayMode = GLUT_DOUBLE | GLUT_RGBA;

	if (OGLT_DEPTH & ogltOptions) {
		DisplayMode |= GLUT_DEPTH;
	}

	if (OGLT_STENCIL & ogltOptions) {
		DisplayMode |= GLUT_STENCIL;
	}
	
	sOgltOptions = ogltOptions;

	glutInitDisplayMode(DisplayMode);
}

bool oglt::glutBackendCreateWindow(uint width, uint height, const char* title, int ogltOptions) {
	glutInitWindowSize(width, height);
	glutCreateWindow(title);

	// Must be done after glut is initialized!
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return false;
	}

	return true;
}

void oglt::glutBackendRun(ICallback* pCallback) {
	if (!pCallback) {
		fprintf(stderr, "%s : callback not specified!\n", __FUNCTION__);
		return;
	}

	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	if (OGLT_DEPTH & sOgltOptions) {
		glEnable(GL_DEPTH_TEST);
	}

	glutSetCursor(GLUT_CURSOR_NONE);

	spCallback = pCallback;
	initCallback();
	glutMainLoop();
}

void oglt::glutBackendSetCursor(int x, int y)
{
	glutWarpPointer(x, y);
}

void oglt::glutBackendSwapBuffers() {
	glutSwapBuffers();
}

static OGLT_BUTTON toOgltButton(int glutMouseButton) {
	switch (glutMouseButton) {
	case GLUT_RIGHT_BUTTON:
		return OGLT_BUTTON_RIGHT;
	case GLUT_MIDDLE_BUTTON:
		return OGLT_BUTTON_MIDDLE;
	case GLUT_LEFT_BUTTON:
		return OGLT_BUTTON_LEFT;
	default:
		cout << "use oglt undefine button" << endl;
	}
	return OGLT_BUTTON_UNDIFINED;
}

static OGLT_BUTTON_STATE toOgltButtonState(int glutState) {
	switch (glutState) {
	case GLUT_DOWN:
		return OGLT_BUTTON_DOWN;
	case GLUT_UP:
		return OGLT_BUTTON_UP;
	default:
		cout << "use oglt undefine button state" << endl;
	}
	return OGLT_BUTTON_STATE_UNDEFINED;
}

static void renderScene() {
	spCallback->renderScene();
}

static void idle() {
	spCallback->renderScene();
}

static void mouse(int glutButton, int glutState, int x, int y) {
	OGLT_BUTTON ogltButton = toOgltButton(glutButton);
	OGLT_BUTTON_STATE ogltState = toOgltButtonState(glutState);
	spCallback->mouse(ogltButton, ogltState, x, y);
}

static void mouseMotion(int x, int y) {
	spCallback->mouseMotion(x, y);
}

static void keyboard(unsigned char glutKey, int x, int y) {
	if (
		((glutKey >= '+') && (glutKey <= '9')) ||
		((glutKey >= 'A') && (glutKey <= 'Z')) ||
		((glutKey >= 'a') && (glutKey <= 'z'))
		) {
		OGLT_KEY key = (OGLT_KEY)glutKey;
		spCallback->keyboard(key);
	}
	else {
		cout << "use oglt undefined key" << endl;
	}
}

static void keyboardUp(unsigned char glutKey, int x, int y) {
	if (
		((glutKey >= '+') && (glutKey <= '9')) ||
		((glutKey >= 'A') && (glutKey <= 'Z')) ||
		((glutKey >= 'a') && (glutKey <= 'z'))
		) {
		OGLT_KEY key = (OGLT_KEY)glutKey;
		spCallback->keyboard(key, OGLT_KEY_RELEASE);
	}
	else {
		cout << "use oglt undefined key" << endl;
	}
}

static void reshape(int width, int height) {
	spCallback->reshape(width, height);
}

void initCallback() {
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(mouseMotion);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
	glutReshapeFunc(reshape);
}