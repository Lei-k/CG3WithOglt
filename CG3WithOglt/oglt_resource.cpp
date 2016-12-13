#include "oglt_resource.h"

using namespace oglt;

Resource* Resource::resource;

Resource* Resource::instance()
{
	if (resource == NULL) {
		resource = new Resource();
	}
	return resource;
}

void Resource::initialize()
{
	shaderPrograms.clear();
	textures.clear();
	materials.clear();

	Shader vtDefault, fgDefault;
	ShaderProgram spDefault;
	vtDefault.loadShader("data/shaders/main_shader.vert", GL_VERTEX_SHADER);
	fgDefault.loadShader("data/shaders/main_shader.frag", GL_FRAGMENT_SHADER);

	spDefault.createProgram();
	spDefault.addShaderToProgram(&vtDefault);
	spDefault.addShaderToProgram(&fgDefault);
	spDefault.linkProgram();

	shaderPrograms.push_back(spDefault);

	Material defMaterial;
	defMaterial.setColorParam(DIFFUSE, glm::vec3(0.5f, 0.5f, 0.5f));
	defMaterial.setShaderProgram(&shaderPrograms[0]);

	materials.push_back(defMaterial);
}

uint Resource::addShaderProgram(ShaderProgram & shaderProgram)
{
	// the shader program id store in Resource class
	uint shaderProgramId = OGLT_INVALID_SHADER_ID;
	FOR(i, ESZ(shaderPrograms)) {
		if (shaderProgram.getProgramID() == shaderPrograms[i].getProgramID()) {
			shaderProgramId = i;
			break;
		}
	}
	if (shaderProgramId == OGLT_INVALID_SHADER_ID) {
		shaderProgramId = ESZ(shaderPrograms);
		shaderPrograms.push_back(shaderProgram);
	}

	return shaderProgramId;
}

ShaderProgram * Resource::getShaderProgram(uint shaderId)
{
	if (shaderId == OGLT_INVALID_SHADER_ID || shaderId >= ESZ(shaderPrograms)) {
		return nullptr;
	}
	return &shaderPrograms[shaderId];
}

uint Resource::addTexture(Texture & texture)
{
	uint textureId = OGLT_INVALID_TEXTURE_ID;
	FOR(i, ESZ(textures)) {
		if (texture.getPath() == textures[i].getPath()) {
			textureId = i;
			break;
		}
	}
	if (textureId == OGLT_INVALID_TEXTURE_ID) {
		textureId = ESZ(textures);
		textures.push_back(texture);
	}
	return textureId;
}

uint oglt::Resource::addTexture(const string & texturePath)
{
	uint textureId = OGLT_INVALID_TEXTURE_ID;
	FOR(i, ESZ(textures)) {
		if (texturePath == textures[i].getPath()) {
			textureId = i;
			break;
		}
	}
	if (textureId == OGLT_INVALID_TEXTURE_ID) {
		textureId = ESZ(textures);
		Texture newTexture;
		newTexture.loadTexture2D(texturePath, true);
		textures.push_back(newTexture);
	}
	return textureId;
}

Texture * Resource::getTexture(uint textureId)
{
	if (textureId == OGLT_INVALID_TEXTURE_ID || textureId >= ESZ(textures)) {
		return nullptr;
	}
	return &textures[textureId];
}

uint Resource::addMaterial(Material & material)
{
	uint materialId = OGLT_INVALID_MATERIAL_ID;
	FOR(i, ESZ(materials)) {
		if (&material == &materials[i]) {
			materialId = i;
		}
	}
	if (materialId == OGLT_INVALID_MATERIAL_ID) {
		materialId = ESZ(materials);
		materials.push_back(material);
		materialMap[material.getName()] = materialId;
	}
	return materialId;
}

Material* Resource::getMaterial(uint materialId)
{
	if (materialId == OGLT_INVALID_MATERIAL_ID || materialId >= ESZ(materials)) {
		return nullptr;
	}
	return &materials[materialId];
}

Material * oglt::Resource::findMaterial(const string & materialName)
{
	if (materialMap.find(materialName) != materialMap.end()) {
		return &materials[materialMap[materialName]];
	}
	return nullptr;
}

Resource::Resource()
{
	initialize();
}

Resource::~Resource()
{
	delete resource;
}
