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

#define SKYBOX_NUM 5

FreeTypeFont ftFont;
FlyingCamera camera;
SceneObject worldTree, cityObj, rObj, testObj, stageObj;
AssimpModel cityModel, rModel;
FbxModel testModel, stageModel;

Shader ortho, font, vtMain, fgMain, dirLight, vtSkin, fgSkin;
ShaderProgram spFont, spMain, spSkin;

vec3 sunDir = vec3(sqrt(2.0f) / 2, -sqrt(2.0f) / 2, 0);

vector<oglt::uint> skyboxIds;
oglt::uint skyboxIndex;

int cameraUpdateMode = OGLT_UPDATEA_CAMERA_WALK | OGLT_UPDATE_CAMERA_ROTATE;

void scene::initScene(oglt::IApp* app) {
	glClearColor(0.1f, 0.3f, 0.7f, 1.0f);

	ortho.loadShader("data/shaders/ortho2D.vert", GL_VERTEX_SHADER);
	font.loadShader("data/shaders/font2D.frag", GL_FRAGMENT_SHADER);
	vtMain.loadShader("data/shaders/main_shader.vert", GL_VERTEX_SHADER);
	fgMain.loadShader("data/shaders/main_shader.frag", GL_FRAGMENT_SHADER);
	dirLight.loadShader("data/shaders/dirLight.frag", GL_FRAGMENT_SHADER);
	vtSkin.loadShader("data/shaders/skinning_shader.vert", GL_VERTEX_SHADER);
	fgSkin.loadShader("data/shaders/skinning_shader.frag", GL_FRAGMENT_SHADER);

	spFont.createProgram();
	spFont.addShaderToProgram(&ortho);
	spFont.addShaderToProgram(&font);
	spFont.linkProgram();

	spMain.createProgram();
	spMain.addShaderToProgram(&vtMain);
	spMain.addShaderToProgram(&dirLight);
	spMain.addShaderToProgram(&fgMain);
	spMain.linkProgram();

	spSkin.createProgram();
	spSkin.addShaderToProgram(&vtSkin);
	spSkin.addShaderToProgram(&dirLight);
	spSkin.addShaderToProgram(&fgSkin);
	spSkin.linkProgram();

	ftFont.loadFont("data/fonts/SugarpunchDEMO.otf", 32);
	ftFont.setShaderProgram(&spFont);

	string skyboxPaths[SKYBOX_NUM * 7] = { "data/skyboxes/elbrus/" , "elbrus_front.jpg" , "elbrus_back.jpg" , "elbrus_right.jpg",
		"elbrus_right.jpg" , "elbrus_top.jpg", "elbrus_top.jpg" ,
		"data/skyboxes/jajlands1/", "jajlands1_ft.jpg", "jajlands1_bk.jpg", "jajlands1_lf.jpg",
		"jajlands1_rt.jpg", "jajlands1_up.jpg", "jajlands1_dn.jpg",
		"data/skyboxes/jf_nuke/", "nuke_ft.tga", "nuke_bk.tga", "nuke_lf.tga",
		"nuke_rt.tga", "nuke_up.tga", "nuke_dn.tga",
		"data/skyboxes/sor_borg/", "borg_ft.tga", "borg_bk.jpg",
		"borg_lf.jpg", "borg_rt.jpg", "borg_up.jpg", "borg_up.jpg" ,
		"data/skyboxes/sor_cwd/", "cwd_ft.jpg", "cwd_bk.jpg", "cwd_lf.jpg",
		"cwd_lf.jpg", "cwd_up.jpg", "cwd_dn.jpg" };

	FOR(i, SKYBOX_NUM) {
		Skybox skybox;
		skybox.load(skyboxPaths[i * 7], skyboxPaths[i * 7 + 1], skyboxPaths[i * 7 + 2], skyboxPaths[i * 7 + 3], skyboxPaths[i * 7 + 4], skyboxPaths[i * 7 + 5], skyboxPaths[i * 7 + 6]);
		skybox.setShaderProgram(&spMain);
		skybox.getLocalTransform()->scale = vec3(10.0f, 10.0f, 10.0f);
		uint skyboxId = Resource::instance()->addSkybox(skybox);
		skyboxIds.push_back(skyboxId);
	}

	skyboxIndex = skyboxIds.size() - 1;

	camera = FlyingCamera(app, vec3(84.0f, 138.0f, 31.0f), vec3(0.0f, 10.0f, 18.0f), vec3(0.0f, 1.0f, 0.0f), 50.0f, 0.01f);
	camera.setMovingKeys('w', 's', 'a', 'd');
	camera.addChild(Resource::instance()->getSkybox(skyboxIndex));

	cityModel.loadModelFromFile("data/models/The City/The City.obj");
	cityObj.addRenderObj(&cityModel);
	cityObj.setShaderProgram(&spMain);

	rModel.loadModelFromFile("data/models/R/R.obj");
	rObj.addRenderObj(&rModel);
	rObj.setShaderProgram(&spMain);
	rObj.getLocalTransform()->position = vec3(292.0f, 130.0f, -180.0f);
	rObj.getLocalTransform()->scale = vec3(40.0f, 40.0f, 40.0f);
	
	worldTree.addChild(&camera);
	//worldTree.addChild(&cityObj);
	cityObj.addChild(&rObj);

	FbxModel::initialize();
	
	stageModel.load("data/models/Rurusyu/scenes/rurusyu.fbx");
	stageObj.addRenderObj(&stageModel);
	stageObj.setShaderProgram(&spSkin);
	worldTree.addChild(&stageObj);

	// Test the fbx model loading
	// developing...
	testModel.load("data/models/TdaJKStyleMaya2/scenes/TdaJKStyle.fbx");
	testObj.addRenderObj(&testModel);
	testObj.setShaderProgram(&spSkin);
	testObj.getLocalTransform()->position = vec3(0.0f, 0.0f, -10.0f);
	testObj.getLocalTransform()->scale = vec3(0.75f, 0.75f, 0.75f);
	stageObj.addChild(&testObj);

	IRenderable::mutexViewMatrix = camera.look();
	IRenderable::mutexProjMatrix = app->getProj();
	IRenderable::mutexOrthoMatrix = app->getOrth();
	IRenderable::mutexSunLightDir = &sunDir;
	IRenderable::mutexCameraPos = &camera.getWorldTransform()->position;

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
}

float animTimer = 0.0f;

void scene::renderScene(oglt::IApp* app) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	worldTree.calcNodeHeirarchyTransform();
	camera.update(cameraUpdateMode);

	animTimer += app->getDeltaTime();
	if (animTimer >= 0.04f) {
		testModel.updateAnimation(animTimer);
		animTimer = 0.0f;
	}

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

	if (app->oneKey('1')) {
		camera.removeChild(Resource::instance()->getSkybox(skyboxIds[skyboxIndex]));
		if (skyboxIndex < skyboxIds.size() - 1) {
			skyboxIndex++;
		}
		else {
			skyboxIndex = 0;
		}
		camera.addChild(Resource::instance()->getSkybox(skyboxIds[skyboxIndex]));
	}

	app->swapBuffers();
}

void scene::releaseScene(oglt::IApp* app) {
	spFont.deleteProgram();

	ortho.deleteShader();
	font.deleteShader();

	spMain.deleteProgram();
	vtMain.deleteShader();
	fgMain.deleteShader();

	spSkin.deleteProgram();
	vtSkin.deleteShader();
	fgSkin.deleteShader();
	dirLight.deleteShader();

	FbxModel::destroyManager();
}