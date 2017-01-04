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
#include "oglt_light.h"

using namespace oglt;
using namespace oglt::scene;

using namespace glm;

#define SKYBOX_NUM 5

FreeTypeFont ftFont;
FlyingCamera camera;
SceneObject worldTree, mikuObj, stageObj;
FbxModel mikuModel, stageModel;

vec3 sunDir = vec3(sqrt(2.0f) / 2, -sqrt(2.0f) / 2, 0);

vector<oglt::uint> skyboxIds;
oglt::uint skyboxIndex;

vector<oglt::uint> cubeMapTextureIds;

int cameraUpdateMode = OGLT_UPDATEA_CAMERA_WALK | OGLT_UPDATE_CAMERA_ROTATE;

AudioSource testSource;

UniformBufferObject uboMatrix;

TwBar* bar;

void bindModelTextureId(FbxModel& fbxModel, MaterialParam textureType, oglt::uint textureId) {
	bool notTexture = textureType != MaterialParam::DIFFUSE && textureType != MaterialParam::CUBE_MAP
		&& textureType != MaterialParam::FUR_TEXTURE;
	if (notTexture) return;

	vector<Mesh>* meshs = fbxModel.getMeshs();
	if (meshs != NULL) {
		for (int i = 0; i < meshs->size(); i++) {
			if (meshs->at(i).mtlMapMode == ALL_SAME) {
				IMaterial* material = Resource::instance()->getMaterial(meshs->at(i).materialId);
				if (material != NULL) {
					material->linkTexture(textureType, textureId);
				}
			}
			else if (meshs->at(i).mtlMapMode == BY_POLYGON) {
				FOR(j, ESZ(meshs->at(i).polygons)) {
					IMaterial* material = Resource::instance()->getMaterial(meshs->at(i).polygons[j].materialId);
					if (material != NULL) {
						material->linkTexture(textureType, textureId);
					}
				}
			}
		}
	}
}

void initShaders() {
	Resource::instance()->addShader("ortho", "data/shaders/ortho2D.vert", GL_VERTEX_SHADER);
	Resource::instance()->addShader("fgFont", "data/shaders/font2D.frag", GL_FRAGMENT_SHADER);
	Resource::instance()->addShader("vtMain", "data/shaders/main_shader.vert", GL_VERTEX_SHADER);
	Resource::instance()->addShader("fgMain", "data/shaders/main_shader.frag", GL_FRAGMENT_SHADER);
	Resource::instance()->addShader("fgDirLight", "data/shaders/dirLight.frag", GL_FRAGMENT_SHADER);
	Resource::instance()->addShader("vtSkin", "data/shaders/skinning_shader.vert", GL_VERTEX_SHADER);
	Resource::instance()->addShader("fgSkin", "data/shaders/skinning_shader.frag", GL_FRAGMENT_SHADER);
	Resource::instance()->addShader("vtReflect", "data/shaders/reflect.vert", GL_VERTEX_SHADER);
	Resource::instance()->addShader("fgReflect", "data/shaders/reflect.frag", GL_FRAGMENT_SHADER);
	Resource::instance()->addShader("fgHandPaint", "data/shaders/hand_painted_shader.frag", GL_FRAGMENT_SHADER);
	Resource::instance()->addShader("vtFurSkin", "data/shaders/fur_skinning_shader.vert", GL_VERTEX_SHADER);
	Resource::instance()->addShader("geFurSkin", "data/shaders/fur_skinning_shader.geom", GL_GEOMETRY_SHADER);
	Resource::instance()->addShader("fgFurSkin", "data/shaders/fur_skinning_shader.frag", GL_FRAGMENT_SHADER);
	Resource::instance()->addShader("fgLights", "data/shaders/Lights.frag", GL_FRAGMENT_SHADER);
	Resource::instance()->addShader("fgLighting", "data/shaders/lighting.frag", GL_FRAGMENT_SHADER);

	Resource::instance()->addShaderProgram("font", 2, "ortho", "fgFont");
	Resource::instance()->addShaderProgram("main", 3, "vtMain", "fgDirLight", "fgMain");
	Resource::instance()->addShaderProgram("skin", 3, "vtSkin", "fgDirLight", "fgSkin");
	Resource::instance()->addShaderProgram("reflect", 3, "vtReflect", "fgLights", "fgReflect");
	Resource::instance()->addShaderProgram("handedSkin", 3, "vtSkin", "fgDirLight", "fgHandPaint");
	Resource::instance()->addShaderProgram("furSkin", 3, "vtFurSkin", "geFurSkin", "fgFurSkin");
	Resource::instance()->addShaderProgram("lightingSkin", 3, "vtSkin", "fgLights", "fgLighting");
}

SceneObject spotLightBall;

void initLights() {
	DirectionalLight directionalLight;
	directionalLight.setParam(LightParam::AMBIENT, vec3(0.023f, 0.023f, 0.023f));
	directionalLight.setParam(LightParam::DIFFUSE, vec3(0.1f, 0.1f, 0.1f));
	directionalLight.setParam(LightParam::SPECULAR, vec3(1.0f, 1.0f, 1.0f));
	Resource::instance()->addDirectionalLight(directionalLight, "directionalLight1");
	directionalLight.getLightParameter()->active = false;
	Resource::instance()->addDirectionalLight(directionalLight, "directionalLight2");
	worldTree.addChild(Resource::instance()->findDirectionalLight("directionalLight1"));
	worldTree.addChild(Resource::instance()->findDirectionalLight("directionalLight2"));
	
	SpotLight spotLight;
	spotLight.setParam(LightParam::AMBIENT, vec3(0.0f, 0.0f, 0.0f));
	spotLight.setParam(LightParam::DIFFUSE, vec3(0.0f, 0.0f, 0.8f));
	spotLight.setParam(LightParam::SPECULAR, vec3(1.0f, 1.0f, 1.0f));
	spotLight.setParam(LightParam::CONSTANT, 1.0f);
	spotLight.setParam(LightParam::LINEAR, 0.09f);
	spotLight.setParam(LightParam::QUADRATIC, 0.032f);
	spotLight.setParam(LightParam::CUTOFF, 0.61f);
	spotLight.setParam(LightParam::OUTER_CUTOFF, 0.5f);
	spotLight.getLightParameter()->position = vec3(0.0f, 0.0f, 0.0f);
	spotLight.getLightParameter()->eulerRotation = vec3(0.0f, 0.0f, 0.0f);
	spotLight.getLightParameter()->active = false;
	
	Resource::instance()->addSpotLight(spotLight, "ball_1_spotLight1");
	spotLight.setParam(LightParam::DIFFUSE, vec3(0.8f, 0.0f, 0.0f));
	spotLight.getLightParameter()->eulerRotation = vec3(0.0f, 0.0f, 90.0f);
	Resource::instance()->addSpotLight(spotLight, "ball_1_spotLight2");
	spotLight.setParam(LightParam::DIFFUSE, vec3(0.0f, 0.8f, 0.0f));
	spotLight.getLightParameter()->eulerRotation = vec3(0.0f, 0.0f, -90.0f);
	Resource::instance()->addSpotLight(spotLight, "ball_1_spotLight3");
	spotLight.setParam(LightParam::DIFFUSE, vec3(0.8f, 0.8f, 0.0f));
	spotLight.getLightParameter()->eulerRotation = vec3(0.0f, 0.0f, 180.0f);
	Resource::instance()->addSpotLight(spotLight, "ball_1_spotLight4");
	spotLight.setParam(LightParam::DIFFUSE, vec3(0.0f, 0.8f, 0.8f));
	spotLight.getLightParameter()->eulerRotation = vec3(90.0f, 0.0f, 0.0f);
	Resource::instance()->addSpotLight(spotLight, "ball_1_spotLight5");
	spotLight.setParam(LightParam::DIFFUSE, vec3(0.8f, 0.0f, 0.8f));
	spotLight.getLightParameter()->eulerRotation = vec3(-90.0f, 0.0f, 0.0f);
	Resource::instance()->addSpotLight(spotLight, "ball_1_spotLight6");

	spotLight.setParam(LightParam::DIFFUSE, vec3(0.8f, 0.8f, 0.8f));
	spotLight.getLightParameter()->active = true;
	spotLight.getLightParameter()->position = vec3(0.0f, 235.0f, 0.0f);
	spotLight.getLightParameter()->eulerRotation = vec3(0.0f, 0.0f, 0.0f);
	spotLight.setParam(LightParam::CUTOFF, 0.92f);
	spotLight.setParam(LightParam::OUTER_CUTOFF, 0.91f);
	spotLight.setParam(LightParam::SPECULAR, vec3(0.0f, 0.0f, 0.0f));
	Resource::instance()->addSpotLight(spotLight, "worldSpotLight1");
	spotLight.getLightParameter()->position = vec3(0.0f, 103.0f, 205.0f);
	spotLight.getLightParameter()->eulerRotation = vec3(1.25f, 0.0f, 0.0f);
	Resource::instance()->addSpotLight(spotLight, "worldSpotLight2");
	spotLight.getLightParameter()->position = vec3(208.0f, 235.0f, -11.0f);
	spotLight.getLightParameter()->eulerRotation = vec3(0.0f, 0.0f, -0.912f);
	Resource::instance()->addSpotLight(spotLight, "worldSpotLight3");
	spotLight.getLightParameter()->position = vec3(-216.0f, 235.0f, -11.0f);
	spotLight.getLightParameter()->eulerRotation = vec3(0.0f, 0.0f, 0.982f);
	Resource::instance()->addSpotLight(spotLight, "worldSpotLight4");
	Resource::instance()->setUpLights();

	spotLightBall.addChild(Resource::instance()->findSpotLight("ball_1_spotLight1"));
	spotLightBall.addChild(Resource::instance()->findSpotLight("ball_1_spotLight2"));
	spotLightBall.addChild(Resource::instance()->findSpotLight("ball_1_spotLight3"));
	spotLightBall.addChild(Resource::instance()->findSpotLight("ball_1_spotLight4"));
	spotLightBall.addChild(Resource::instance()->findSpotLight("ball_1_spotLight5"));
	spotLightBall.addChild(Resource::instance()->findSpotLight("ball_1_spotLight6"));
	spotLightBall.getLocalTransform()->position = vec3(0.0f, 70.0f, 0.0f);
	worldTree.addChild(&spotLightBall);

	worldTree.addChild(Resource::instance()->findSpotLight("worldSpotLight1"));
	worldTree.addChild(Resource::instance()->findSpotLight("worldSpotLight2"));
	worldTree.addChild(Resource::instance()->findSpotLight("worldSpotLight3"));
	worldTree.addChild(Resource::instance()->findSpotLight("worldSpotLight4"));
}

vec3 eulerVector = vec3(0.0f);

void initTwBar() {
	bar = TwNewBar("TweakBar");
	TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar with GLUT and OpenGL.' "); // Message added to the help bar.
	TwDefine(" TweakBar size='200 350' color='96 216 224'");
	TwStructMember positionMembers[] = {
		{ "X", TW_TYPE_FLOAT, offsetof(vec3, x), "" },
		{ "Y", TW_TYPE_FLOAT, offsetof(vec3, y), "" },
		{ "Z", TW_TYPE_FLOAT, offsetof(vec3, z), "" } };
	TwStructMember eulerMembers[] = {
		{ "X", TW_TYPE_FLOAT, offsetof(vec3, x), "step=0.002" },
		{ "Y", TW_TYPE_FLOAT, offsetof(vec3, y), "step=0.002" },
		{ "Z", TW_TYPE_FLOAT, offsetof(vec3, z), "step=0.002" } };
	TwType positionType = TwDefineStruct("Position", positionMembers, 3, sizeof(vec3), NULL, NULL);
	TwType eulerType = TwDefineStruct("Euler", eulerMembers, 3, sizeof(vec3), NULL, NULL);
	TwStructMember directionalLightMembers[] = {
		{ "Active", TW_TYPE_BOOLCPP, offsetof(DirectionalLightParameter, active), ""},
		{ "Position", positionType, offsetof(DirectionalLightParameter, position), ""},
		{ "Rotation", eulerType, offsetof(DirectionalLightParameter, eulerRotation), ""},
		{ "Ambient Color", TW_TYPE_COLOR3F, offsetof(DirectionalLightParameter, ambient), ""},
		{ "Diffuse Color", TW_TYPE_COLOR3F, offsetof(DirectionalLightParameter, diffuse), ""},
		{ "Specular Color", TW_TYPE_COLOR3F, offsetof(DirectionalLightParameter, specular), ""}
	};
	
	TwStructMember spotLightMembers[] = {
		{ "Active", TW_TYPE_BOOLCPP, offsetof(SpotLightParameter, active), "" },
		{ "Position", positionType, offsetof(SpotLightParameter, position), "" },
		{ "Rotation", eulerType, offsetof(SpotLightParameter, eulerRotation), "" },
		{ "Ambient Color", TW_TYPE_COLOR3F, offsetof(SpotLightParameter, ambient), "" },
		{ "Diffuse Color", TW_TYPE_COLOR3F, offsetof(SpotLightParameter, diffuse), "" },
		{ "Specular Color", TW_TYPE_COLOR3F, offsetof(SpotLightParameter, specular), "" },
		{ "Cutoff", TW_TYPE_FLOAT, offsetof(SpotLightParameter, cutOff), "step=0.001"},
		{ "Out Cutoff", TW_TYPE_FLOAT, offsetof(SpotLightParameter, outerCutOff), "step=0.001"},
		{ "Constant", TW_TYPE_FLOAT, offsetof(SpotLightParameter, constant), "step=0.001"},
		{ "Linear", TW_TYPE_FLOAT, offsetof(SpotLightParameter, linear), "step=0.001" },
		{ "quadratic", TW_TYPE_FLOAT, offsetof(SpotLightParameter, quadratic), "step=0.001" }
	};

	TwType directionalLightType = TwDefineStruct("Directional Light", directionalLightMembers, 5, sizeof(DirectionalLightParameter), NULL, NULL);
	TwType spotLightType = TwDefineStruct("Spot Light", spotLightMembers, 11, sizeof(SpotLightParameter), NULL, NULL);

	DirectionalLight* directinalLight = Resource::instance()->findDirectionalLight("directionalLight1");
	if (directinalLight != NULL) {
		TwAddVarRW(bar, "DirectionalLight1", directionalLightType, directinalLight->getLightParameter(), "");
	}
	directinalLight = Resource::instance()->findDirectionalLight("directionalLight2");
	if (directinalLight != NULL) {
		TwAddVarRW(bar, "DirectionalLight2", directionalLightType, directinalLight->getLightParameter(), "");
	}

	TwAddVarRW(bar, "SpotLightBall_1_Pos", positionType, &spotLightBall.getLocalTransform()->position, "Group='SpotLightBall_1' Label='Position'");
	TwAddVarRW(bar, "SpotLightBall_1_Rot", eulerType, &eulerVector, "Group='SpotLightBall_1' Label='Rotate'");
	TwAddVarRW(bar, "SpotLight_1", spotLightType, Resource::instance()->findSpotLight("ball_1_spotLight1")->getLightParameter(), "Group='SpotLightBall_1' Label='SpotLight1'");
	TwAddVarRW(bar, "SpotLight_2", spotLightType, Resource::instance()->findSpotLight("ball_1_spotLight2")->getLightParameter(), "Group='SpotLightBall_1' Label='SpotLight2'");
	TwAddVarRW(bar, "SpotLight_3", spotLightType, Resource::instance()->findSpotLight("ball_1_spotLight3")->getLightParameter(), "Group='SpotLightBall_1' Label='SpotLight3'");
	TwAddVarRW(bar, "SpotLight_4", spotLightType, Resource::instance()->findSpotLight("ball_1_spotLight4")->getLightParameter(), "Group='SpotLightBall_1' Label='SpotLight4'");
	TwAddVarRW(bar, "SpotLight_5", spotLightType, Resource::instance()->findSpotLight("ball_1_spotLight5")->getLightParameter(), "Group='SpotLightBall_1' Label='SpotLight5'");
	TwAddVarRW(bar, "SpotLight_6", spotLightType, Resource::instance()->findSpotLight("ball_1_spotLight6")->getLightParameter(), "Group='SpotLightBall_1' Label='SpotLight6'");
	TwAddVarRW(bar, "World Spot Light1", spotLightType, Resource::instance()->findSpotLight("worldSpotLight1")->getLightParameter(), "");
	TwAddVarRW(bar, "World Spot Light2", spotLightType, Resource::instance()->findSpotLight("worldSpotLight2")->getLightParameter(), "");
	TwAddVarRW(bar, "World Spot Light3", spotLightType, Resource::instance()->findSpotLight("worldSpotLight3")->getLightParameter(), "");
	TwAddVarRW(bar, "World Spot Light4", spotLightType, Resource::instance()->findSpotLight("worldSpotLight4")->getLightParameter(), "");
}

void scene::initScene(oglt::IApp* app) {
	glClearColor(0.1f, 0.3f, 0.7f, 1.0f);

	initShaders();

	ftFont.loadFont("data/fonts/SugarpunchDEMO.otf", 32);
	ftFont.setShaderProgram(Resource::instance()->findShaderProgram("font"));

	string skyboxPaths[SKYBOX_NUM * 7] = { "data/skyboxes/elbrus/" , "elbrus_front.jpg" , "elbrus_back.jpg" , "elbrus_right.jpg",
		"elbrus_left.jpg" , "elbrus_top.jpg", "elbrus_top.jpg" ,
		"data/skyboxes/jajlands1/", "jajlands1_ft.jpg", "jajlands1_bk.jpg", "jajlands1_lf.jpg",
		"jajlands1_rt.jpg", "jajlands1_up.jpg", "jajlands1_dn.jpg",
		"data/skyboxes/jf_nuke/", "nuke_ft.tga", "nuke_bk.tga", "nuke_lf.tga",
		"nuke_rt.tga", "nuke_up.tga", "nuke_dn.tga",
		"data/skyboxes/sor_borg/", "borg_ft.jpg", "borg_bk.jpg",
		"borg_lf.jpg", "borg_rt.jpg", "borg_up.jpg", "borg_up.jpg" ,
		"data/skyboxes/sor_cwd/", "cwd_ft.jpg", "cwd_bk.jpg", "cwd_lf.jpg",
		"cwd_lf.jpg", "cwd_up.jpg", "cwd_dn.jpg" };

	FOR(i, SKYBOX_NUM) {
		Skybox skybox;
		skybox.load(skyboxPaths[i * 7], skyboxPaths[i * 7 + 1], skyboxPaths[i * 7 + 2], skyboxPaths[i * 7 + 3], skyboxPaths[i * 7 + 4], skyboxPaths[i * 7 + 5], skyboxPaths[i * 7 + 6]);
		skybox.setShaderProgram(Resource::instance()->findShaderProgram("main"));
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
	stageObj.setShaderProgram(Resource::instance()->findShaderProgram("reflect"));
	worldTree.addChild(&stageObj);

	mikuModel.load("data/models/TdaJKStyleMaya2/scenes/TdaJKStyle.fbx");
	mikuObj.addRenderObj(&mikuModel);
	mikuObj.setShaderProgram(Resource::instance()->findShaderProgram("handedSkin"));
	mikuObj.getLocalTransform()->scale = vec3(0.75f, 0.75f, 0.75f);
	worldTree.addChild(&mikuObj);

	IRenderable::mutexViewMatrix = camera.look();
	IRenderable::mutexProjMatrix = app->getProj();
	IRenderable::mutexOrthoMatrix = app->getOrth();
	IRenderable::mutexSunLightDir = &sunDir;
	IRenderable::mutexCameraPos = &camera.getWorldTransform()->position;

	// the skybox index is same as cube map texture index
	bindModelTextureId(stageModel, CUBE_MAP, cubeMapTextureIds[skyboxIndex]);
	bindModelTextureId(mikuModel, CUBE_MAP, cubeMapTextureIds[skyboxIndex]);

	Texture furTexture;
	furTexture.loadTexture2D("data/textures/furFill.png");
	uint furTextureId = Resource::instance()->addTexture(furTexture);
	bindModelTextureId(mikuModel, FUR_TEXTURE, furTextureId);

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);

	testSource.load("data/musics/Tell Your World Dance.wav");

	initLights();
	uboMatrix.createUniformBuffer();
	uboMatrix.addData(IRenderable::mutexProjMatrix, sizeof(glm::mat4));
	uboMatrix.addData(IRenderable::mutexViewMatrix, sizeof(glm::mat4));
	uboMatrix.uploadBufferData(0, GL_DYNAMIC_DRAW);
	uboMatrix.updateBuffer();

	initTwBar();
}

float animTimer = 0.0f;
bool playAnimation = false;
int switchShaderProgram = 0;

void oglt::scene::updateScene(IApp * app)
{
	worldTree.calcNodeHeirarchyTransform();

	// just for testing
	if (app->oneKey('r') || app->oneKey('R')) {
		if (cameraUpdateMode & OGLT_UPDATE_CAMERA_ROTATE) {
			cameraUpdateMode ^= OGLT_UPDATE_CAMERA_ROTATE;
			app->setCursor(OGLT_CURSOR_ARROW);
		}
		else {
			cameraUpdateMode |= OGLT_UPDATE_CAMERA_ROTATE;
			app->setCursor(OGLT_CURSOR_NONE);
		}
	}

	if (app->oneKey('t') || app->oneKey('T')) {
		mikuModel.setTimer(-0.4f);
		testSource.rewind();
		testSource.play();
	}

	if (app->oneKey('y') || app->oneKey('Y')) {
		playAnimation = !playAnimation;
	}

	if (app->oneKey('e') || app->oneKey('E')) {
		switchShaderProgram++;
		if (switchShaderProgram >= 4) {
			switchShaderProgram = 0;
		}
		ShaderProgram* program = nullptr;
		switch (switchShaderProgram) {
		case 0:
			program = Resource::instance()->findShaderProgram("skin");
			break;
		case 1:
			program = Resource::instance()->findShaderProgram("handedSkin");
			break;
		case 2:
			program = Resource::instance()->findShaderProgram("furSkin");
			break;
		case 3:
			program = Resource::instance()->findShaderProgram("lightingSkin");
			break;
		}
		if (program != nullptr) {
			mikuModel.setShaderProgram(program);
		}
	}

	if (playAnimation) {
		animTimer += app->getDeltaTime();
		if (animTimer >= 0.04f) {
			mikuModel.updateAnimation(animTimer);
			animTimer = 0.0f;
		}
	}

	spotLightBall.getLocalTransform()->rotation = quat(eulerVector);
}

void scene::renderScene(oglt::IApp* app) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	uboMatrix.updateBuffer();
	Resource::instance()->updateLights();

	// I put the camera update function in render scene
	// because in the camera update function I use the app->getFrameDeltaTime
	// function it's return delta time of per render scene.
	// it's more smooth then delta time. the camera is sensitive so I do that in
	// render scene. rather then in update scene.
	camera.update(cameraUpdateMode);

	worldTree.render(OGLT_RENDER_CHILDREN);

	ShaderProgram* spFont = Resource::instance()->findShaderProgram("font");
	if (spFont != NULL) {
		spFont->useProgram();
		spFont->setUniform("matrices.projMatrix", app->getOrth());
		spFont->setUniform("vColor", vec4(1.0f, 1.0f, 1.0f, 1.0f));
	}
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
		bindModelTextureId(stageModel, CUBE_MAP, cubeMapTextureIds[skyboxIndex]);
		bindModelTextureId(mikuModel, CUBE_MAP, cubeMapTextureIds[skyboxIndex]);
	}

	if (!(cameraUpdateMode & OGLT_UPDATE_CAMERA_ROTATE)) {
		TwDraw();
	}

	app->swapBuffers();
}

void scene::releaseScene(oglt::IApp* app) {
	FbxModel::destroyManager();

	TwDeleteBar(bar);
}
