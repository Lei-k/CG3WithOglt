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
#include "oglt_audio_source.h"

using namespace oglt;
using namespace oglt::scene;

using namespace glm;

#define SKYBOX_NUM 5

FreeTypeFont ftFont;
FlyingCamera camera;
SceneObject worldTree, testObj, stageObj;
FbxModel testModel, stageModel;

Shader ortho, font, vtMain, fgMain, dirLight, vtSkin, fgSkin, vtReflect, fgReflect, fgHandPaint;
ShaderProgram spFont, spMain, spSkin, spReflect, spHandSkin;

vec3 sunDir = vec3(sqrt(2.0f) / 2, -sqrt(2.0f) / 2, 0);

vector<oglt::uint> skyboxIds;
oglt::uint skyboxIndex;

vector<oglt::uint> cubeMapTextureIds;

int cameraUpdateMode = OGLT_UPDATEA_CAMERA_WALK | OGLT_UPDATE_CAMERA_ROTATE;

AudioSource testSource;

void bindModelCubeMapId(FbxModel& fbxModel, oglt::uint cubeMapTextureId) {
	vector<Mesh>* meshs = fbxModel.getMeshs();
	if (meshs != NULL) {
		for (int i = 0; i < meshs->size(); i++) {
			if (meshs->at(i).mtlMapMode == ALL_SAME) {
				IMaterial* material = Resource::instance()->getMaterial(meshs->at(i).materialId);
				if (material != NULL) {
					material->linkTexture(MaterialParam::CUBE_MAP, cubeMapTextureId);
				}
			}
			else if (meshs->at(i).mtlMapMode == BY_POLYGON) {
				FOR(j, ESZ(meshs->at(i).polygons)) {
					IMaterial* material = Resource::instance()->getMaterial(meshs->at(i).polygons[j].materialId);
					if (material != NULL) {
						material->linkTexture(MaterialParam::CUBE_MAP, cubeMapTextureId);
					}
				}
			}
		}
	}
}

void scene::initScene(oglt::IApp* app) {
	glClearColor(0.1f, 0.3f, 0.7f, 1.0f);

	ortho.loadShader("data/shaders/ortho2D.vert", GL_VERTEX_SHADER);
	font.loadShader("data/shaders/font2D.frag", GL_FRAGMENT_SHADER);
	vtMain.loadShader("data/shaders/main_shader.vert", GL_VERTEX_SHADER);
	fgMain.loadShader("data/shaders/main_shader.frag", GL_FRAGMENT_SHADER);
	dirLight.loadShader("data/shaders/dirLight.frag", GL_FRAGMENT_SHADER);
	vtSkin.loadShader("data/shaders/skinning_shader.vert", GL_VERTEX_SHADER);
	fgSkin.loadShader("data/shaders/skinning_shader.frag", GL_FRAGMENT_SHADER);
	vtReflect.loadShader("data/shaders/reflect.vert", GL_VERTEX_SHADER);
	fgReflect.loadShader("data/shaders/reflect.frag", GL_FRAGMENT_SHADER);
	fgHandPaint.loadShader("data/shaders/hand_painted_shader.frag", GL_FRAGMENT_SHADER);

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

	spReflect.createProgram();
	spReflect.addShaderToProgram(&vtReflect);
	spReflect.addShaderToProgram(&dirLight);
	spReflect.addShaderToProgram(&fgReflect);
	spReflect.linkProgram();

	spHandSkin.createProgram();
	spHandSkin.addShaderToProgram(&vtSkin);
	spHandSkin.addShaderToProgram(&dirLight);
	spHandSkin.addShaderToProgram(&fgHandPaint);
	spHandSkin.linkProgram();

	ftFont.loadFont("data/fonts/SugarpunchDEMO.otf", 32);
	ftFont.setShaderProgram(&spFont);

	string skyboxPaths[SKYBOX_NUM * 7] = { "data/skyboxes/elbrus/" , "elbrus_front.jpg" , "elbrus_back.jpg" , "elbrus_right.jpg",
		"elbrus_left.jpg" , "elbrus_top.jpg", "elbrus_top.jpg" ,
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

		Texture cubeMapTexture;
		cubeMapTexture.loadTexture(skyboxPaths[i * 7] + skyboxPaths[i * 7 + 1], skyboxPaths[i * 7] + skyboxPaths[i * 7 + 2],
			skyboxPaths[i * 7] + skyboxPaths[i * 7 + 4], skyboxPaths[i * 7] + skyboxPaths[i * 7 + 3], skyboxPaths[i * 7] + skyboxPaths[i * 7 + 5], skyboxPaths[i * 7] + skyboxPaths[i * 7 + 6]);
		uint cubeMapId = Resource::instance()->addTexture(cubeMapTexture);
		cubeMapTextureIds.push_back(cubeMapId);
	}

	skyboxIndex = skyboxIds.size() - 1;

	camera = FlyingCamera(app, vec3(15.0f, 158.0f, 469.0f), vec3(0.0f, 170.0f, 500.0f), vec3(0.0f, 1.0f, 0.0f), 100.0f, 0.01f);
	camera.setMovingKeys('w', 's', 'a', 'd');
	camera.addChild(Resource::instance()->getSkybox(skyboxIndex));

	FbxModel::initialize();
	
	worldTree.addChild(&camera);
	
	stageModel.load("data/models/Rurusyu/scenes/rurusyu.fbx");
	stageObj.addRenderObj(&stageModel);
	stageObj.setShaderProgram(&spReflect);
	worldTree.addChild(&stageObj);

	// Test the fbx model loading
	// developing...
	testModel.load("data/models/TdaJKStyleMaya2/scenes/TdaJKStyle.fbx");
	testObj.addRenderObj(&testModel);
	testObj.setShaderProgram(&spHandSkin);
	testObj.getLocalTransform()->position = vec3(0.0f, 0.0f, -10.0f);
	testObj.getLocalTransform()->scale = vec3(0.75f, 0.75f, 0.75f);
	worldTree.addChild(&testObj);

	IRenderable::mutexViewMatrix = camera.look();
	IRenderable::mutexProjMatrix = app->getProj();
	IRenderable::mutexOrthoMatrix = app->getOrth();
	IRenderable::mutexSunLightDir = &sunDir;
	IRenderable::mutexCameraPos = &camera.getWorldTransform()->position;

	// the skybox index is same as cube map texture index
	bindModelCubeMapId(stageModel, cubeMapTextureIds[skyboxIndex]);
	bindModelCubeMapId(testModel, cubeMapTextureIds[skyboxIndex]);

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);

	testSource.load("data/musics/Tell Your World Dance.wav");
}

float animTimer = 0.0f;
bool playAnimation = false;

void oglt::scene::updateScene(IApp * app)
{
	worldTree.calcNodeHeirarchyTransform();

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
		testModel.setTimer(-0.4f);
		testSource.rewind();
		testSource.play();
	}

	if (app->oneKey('y') || app->oneKey('Y')) {
		playAnimation = !playAnimation;
	}

	if (playAnimation) {
		animTimer += app->getDeltaTime();
		if (animTimer >= 0.04f) {
			testModel.updateAnimation(animTimer);
			animTimer = 0.0f;
		}
	}
}

void scene::renderScene(oglt::IApp* app) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// I put the camera update function in render scene
	// because in the camera update function I use the app->getFrameDeltaTime
	// function it's return delta time of per render scene.
	// it's more smooth then delta time. the camera is sensitive so I do that in
	// render scene. rather then in update scene.
	camera.update(cameraUpdateMode);

	worldTree.render(OGLT_RENDER_CHILDREN);

	spFont.useProgram();
	spFont.setUniform("matrices.projMatrix", app->getOrth());
	spFont.setUniform("vColor", vec4(1.0f, 1.0f, 1.0f, 1.0f));
	glDisable(GL_DEPTH_TEST);

	uint w, h;
	app->getViewport(w, h);
	ftFont.printFormatted(20, h - 35, 24, "UPS: %d", app->getUps());
	ftFont.printFormatted(20, h - 70, 24, "FPS: %d", app->getFps());
	ftFont.printFormatted(20, h - 100, 20, "X: %.2f", camera.getWorldTransform()->position.x);
	ftFont.printFormatted(20, h - 123, 20, "Y: %.2f", camera.getWorldTransform()->position.y);
	ftFont.printFormatted(20, h - 146, 20, "Z: %.2f", camera.getWorldTransform()->position.z);
	ftFont.print("OgltApp : https://github.com/Lei-k/oglt_app", 10, 15, 20);
	ftFont.render();

	glEnable(GL_DEPTH_TEST);

	// I put change skybox code in render scene
	// because render scene and update scene are
	// called by difference threads.
	// if I remove some thing in world three
	// in update scene. it may cause nullptr problem when
	// render scene render some thing in world tree
	// and this operation is not weighted, so it's ok to put it
	// in render scene
	if (app->oneKey('1')) {
		camera.removeChild(Resource::instance()->getSkybox(skyboxIds[skyboxIndex]));
		if (skyboxIndex < skyboxIds.size() - 1) {
			skyboxIndex++;
		}
		else {
			skyboxIndex = 0;
		}
		camera.addChild(Resource::instance()->getSkybox(skyboxIds[skyboxIndex]));
		bindModelCubeMapId(stageModel, cubeMapTextureIds[skyboxIndex]);
		bindModelCubeMapId(testModel, cubeMapTextureIds[skyboxIndex]);
	}

	app->swapBuffers();
}

void scene::releaseScene(oglt::IApp* app) {
	spFont.deleteProgram();
	spSkin.deleteProgram();
	spMain.deleteProgram();
	spReflect.deleteProgram();
	spHandSkin.deleteProgram();

	ortho.deleteShader();
	font.deleteShader();
	
	vtMain.deleteShader();
	fgMain.deleteShader();

	vtSkin.deleteShader();
	fgSkin.deleteShader();
	vtReflect.deleteShader();
	fgReflect.deleteShader();
	fgHandPaint.deleteShader();

	dirLight.deleteShader();

	FbxModel::destroyManager();
}
