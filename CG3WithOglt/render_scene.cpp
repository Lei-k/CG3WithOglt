#include "render_scene.h"

#include "oglt_util.h"
#include "std_util.h"

#include "oglt_shader.h"
#include "oglt_freetypefont.h"
#include "oglt_skybox.h"
#include "oglt_camera.h"
#include "oglt_assimp_model.h"
#include "oglt_fbx_model.h"
#include "oglt_scene_object.h"

using namespace oglt;
using namespace oglt::scene;

using namespace glm;

FreeTypeFont ftFont;
Skybox skybox;
FlyingCamera camera;
SceneObject worldTree, cityObj, rObj, testObj;
AssimpModel cityModel, rModel;
FbxModel testModel;

Shader ortho, font, vtMain, fgMain, dirLight, vtFbx, fgFbx;
ShaderProgram spFont, spMain, spFbx;

vec3 sunDir = vec3(sqrt(2.0f) / 2, -sqrt(2.0f) / 2, 0);

int cameraUpdateMode = OGLT_UPDATEA_CAMERA_WALK | OGLT_UPDATE_CAMERA_ROTATE;

void scene::initScene(oglt::IApp* app) {
	glClearColor(0.1f, 0.3f, 0.7f, 1.0f);

	ortho.loadShader("data/shaders/ortho2D.vert", GL_VERTEX_SHADER);
	font.loadShader("data/shaders/font2D.frag", GL_FRAGMENT_SHADER);
	vtMain.loadShader("data/shaders/main_shader.vert", GL_VERTEX_SHADER);
	fgMain.loadShader("data/shaders/main_shader.frag", GL_FRAGMENT_SHADER);
	dirLight.loadShader("data/shaders/dirLight.frag", GL_FRAGMENT_SHADER);
	vtFbx.loadShader("data/shaders/fbx_shader.vert", GL_VERTEX_SHADER);
	fgFbx.loadShader("data/shaders/fbx_shader.frag", GL_FRAGMENT_SHADER);

	spFont.createProgram();
	spFont.addShaderToProgram(&ortho);
	spFont.addShaderToProgram(&font);
	spFont.linkProgram();

	spMain.createProgram();
	spMain.addShaderToProgram(&vtMain);
	spMain.addShaderToProgram(&dirLight);
	spMain.addShaderToProgram(&fgMain);
	spMain.linkProgram();

	spFbx.createProgram();
	spFbx.addShaderToProgram(&vtFbx);
	spFbx.addShaderToProgram(&dirLight);
	spFbx.addShaderToProgram(&fgFbx);
	spFbx.linkProgram();

	ftFont.loadFont("data/fonts/SugarpunchDEMO.otf", 32);
	ftFont.setShaderProgram(&spFont);
	
	skybox.load("data/skyboxes/jajlands1/", "jajlands1_ft.jpg", "jajlands1_bk.jpg", "jajlands1_lf.jpg", "jajlands1_rt.jpg", "jajlands1_up.jpg", "jajlands1_dn.jpg");
	skybox.setShaderProgram(&spMain);
	skybox.getLocalTransform()->scale = vec3(10.0f, 10.0f, 10.0f);

	camera = FlyingCamera(app, vec3(84.0f, 138.0f, 31.0f), vec3(0.0f, 10.0f, 18.0f), vec3(0.0f, 1.0f, 0.0f), 50.0f, 0.01f);
	camera.setMovingKeys('w', 's', 'a', 'd');
	camera.addChild(&skybox);

	cityModel.loadModelFromFile("data/models/The City/The City.obj");
	cityObj.addRenderObj(&cityModel);
	cityObj.setShaderProgram(&spMain);

	rModel.loadModelFromFile("data/models/R/R.obj");
	rObj.addRenderObj(&rModel);
	rObj.setShaderProgram(&spMain);
	rObj.getLocalTransform()->position = vec3(292.0f, 130.0f, -180.0f);
	rObj.getLocalTransform()->scale = vec3(40.0f, 40.0f, 40.0f);
	
	worldTree.addChild(&camera);
	worldTree.addChild(&cityObj);
	cityObj.addChild(&rObj);

	FbxModel::initialize();
	
	// Test the fbx model loading
	// developing...
	testModel.load("data/models/TdaJKStyleMaya2/scenes/TdaJKStyle.fbx");
	testModel.setShaderProgram(&spFbx);
	testObj.addRenderObj(&testModel);
	testObj.setShaderProgram(&spFbx);
	testObj.getLocalTransform()->position = vec3(0.0f, 50.0f, 0.0f);
	cityObj.addChild(&testObj);

	IRenderable::mutexViewMatrix = camera.look();
	IRenderable::mutexProjMatrix = app->getProj();
	IRenderable::mutexOrthoMatrix = app->getOrth();
	IRenderable::mutexSunLightDir = &sunDir;

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
}

void scene::renderScene(oglt::IApp* app) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	worldTree.calcNodeHeirarchyTransform();
	camera.update(cameraUpdateMode);

	testModel.updateAnimation(app->getDeltaTime());

	worldTree.render(OGLT_RENDER_CHILDREN);

	spFont.useProgram();
	spFont.setUniform("matrices.projMatrix", app->getOrth());
	spFont.setUniform("vColor", vec4(1.0f, 1.0f, 1.0f, 1.0f));
	glDisable(GL_DEPTH_TEST);

	uint w, h;
	app->getViewport(w, h);
	ftFont.printFormatted(20, h - 35, 24, "FPS: %d", app->getFps());
	ftFont.printFormatted(20, h - 65, 20, "X: %.2f", camera.getWorldTransform()->position.x);
	ftFont.printFormatted(20, h - 88, 20, "Y: %.2f", camera.getWorldTransform()->position.y);
	ftFont.printFormatted(20, h - 111, 20, "Z: %.2f", camera.getWorldTransform()->position.z);
	ftFont.print("OgltApp : https://github.com/Lei-k/oglt_app", 10, 15, 20);
	ftFont.render();

	glEnable(GL_DEPTH_TEST);

	// just for testing
	if (app->oneKey('r') || app->oneKey('R')) {
		if (cameraUpdateMode & OGLT_UPDATE_CAMERA_ROTATE) {
			cameraUpdateMode ^= OGLT_UPDATE_CAMERA_ROTATE;
		}
		else {
			cameraUpdateMode |= OGLT_UPDATE_CAMERA_ROTATE;
		}
	}

	if (app->oneKey('t') || app->oneKey('T')) {
		testModel.setTimer(0.0f);
	}

	app->swapBuffers();
}

void scene::releaseScene(oglt::IApp* app) {
	spFont.deleteProgram();

	ortho.deleteShader();
	font.deleteShader();

	skybox.deleteSkybox();

	spMain.deleteProgram();
	vtMain.deleteShader();
	fgMain.deleteShader();

	spFbx.deleteProgram();
	vtFbx.deleteShader();
	fgFbx.deleteShader();
	dirLight.deleteShader();

	FbxModel::destroyManager();
}