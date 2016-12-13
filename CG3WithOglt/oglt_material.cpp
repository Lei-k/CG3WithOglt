#include "oglt_material.h"

#include "oglt_irenderable.h"
#include "oglt_resource.h"

using namespace oglt;

Material::Material()
{
	enableAmbient = false;
	enableDiffuse = false;
	enableSpecular = false;
	enableEmissive = false;
	enableTransparencyFactor = false;
	enableShininessFactor = false;
	enableReflectionFactor = false;

	shaderProgram = NULL;

	diffuseTextureId = OGLT_INVALID_TEXTURE_INDEX;
	specularTextureId = OGLT_INVALID_TEXTURE_INDEX;
	toonTextureId = OGLT_INVALID_TEXTURE_INDEX;
}

Material::~Material()
{

}

void Material::setColorParam(MaterialParam param, glm::vec3& color)
{
	switch (param) {
	case AMBIENT:
		ambient = color;
		enableAmbient = true;
		break;
	case DIFFUSE:
		diffuse = color;
		enableDiffuse = true;
		break;
	case SPECULAR:
		specular = color;
		enableSpecular = true;
		break;
	case EMISSIVE:
		emissive = color;
		enableEmissive = true;
		break;
	default:
		fprintf(stderr, "Error: Use undefind \n");
	}
}

void Material::setFactorParam(MaterialParam param, float factor)
{
	switch (param) {
	case TRANSPARENCY_FACTOR:
		transparencyFactor = factor;
		enableTransparencyFactor = true;
		break;
	case SHININESS_FACTOR:
		shininessFactor = factor;
		enableShininessFactor = true;
		break;
	case REFLECTION_FACTOR:
		reflectionFactor = factor;
		enableReflectionFactor = true;
		break;
	default:
		fprintf(stderr, "Error: Use undefined factor\n");
	}
}

glm::vec3 * oglt::Material::getColorParam(MaterialParam param)
{
	switch (param) {
	case AMBIENT:
		if (enableAmbient)
			return &ambient;
		break;
	case DIFFUSE:
		if (enableDiffuse)
			return &diffuse;
		break;
	case SPECULAR:
		if (enableSpecular)
			return &specular;
		break;
	case EMISSIVE:
		if (enableEmissive)
			return &emissive;
		break;
	default:
		fprintf(stderr, "Error: Use undefind \n");
	}
	fprintf(stderr, "Error: Use unenable \n");
	return nullptr;
}

float Material::getFactorParam(MaterialParam param)
{
	switch (param) {
	case TRANSPARENCY_FACTOR:
		if (enableTransparencyFactor)
			return transparencyFactor;
		break;
	case SHININESS_FACTOR:
		if (enableShininessFactor)
			return shininessFactor;
		break;
	case REFLECTION_FACTOR:
		if (enableReflectionFactor)
			return reflectionFactor;
		break;
	default:
		fprintf(stderr, "Error: Use undefined factor\n");
	}
	fprintf(stderr, "Error: Use unenable factor\n");
	return 0.0f;
}

void oglt::Material::linkTexture(MaterialParam param, uint textureId)
{
	switch (param) {
	case DIFFUSE:
		diffuseTextureId = textureId;
		break;
	case SPECULAR:
		specularTextureId = textureId;
		break;
	case TOON:
		toonTextureId = textureId;
		break;
	default:
		fprintf(stderr, "Error: Use undefined texture param\n");
	}
}

uint oglt::Material::getTextureId(MaterialParam param)
{
	switch (param) {
	case DIFFUSE:
		return diffuseTextureId;
	case SPECULAR:
		return specularTextureId;
	case TOON:
		return toonTextureId;
	default:
		fprintf(stderr, "Error: Use undefined texture param\n");
	}
	return OGLT_INVALID_TEXTURE_INDEX;
}


void oglt::Material::setShaderProgram(ShaderProgram * shaderProgram)
{
	this->shaderProgram = shaderProgram;
}

ShaderProgram * oglt::Material::getShaderProgram()
{
	return shaderProgram;
}

void oglt::Material::setBoneTransforms(const vector<glm::mat4>& boneTransforms)
{
	this->boneTransforms = boneTransforms;
}

void oglt::Material::setName(const string & materialName)
{
	this->materialName = materialName;
}

const string & oglt::Material::getName()
{
	return materialName;
}

void oglt::Material::useMaterial()
{
	if (shaderProgram == NULL)
		return;

	if (shaderProgram != IRenderable::mutexShaderProgram) {
		shaderProgram->useProgram();
		shaderProgram->setUniform("matrices.viewMatrix", IRenderable::mutexViewMatrix);
		shaderProgram->setUniform("matrices.projMatrix", IRenderable::mutexProjMatrix);
		shaderProgram->setModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", IRenderable::mutexModelMatrix);
		shaderProgram->setUniform("sunLight.vColor", glm::vec3(1.0f, 1.0f, 1.0f));
		shaderProgram->setUniform("sunLight.vDirection", IRenderable::mutexSunLightDir);
		shaderProgram->setUniform("sunLight.fAmbient", 1.0f);

		char buf[50];
		FOR(i, ESZ(boneTransforms)) {
			sprintf(buf, "gBones[%d]", i);
			shaderProgram->setUniform(buf, boneTransforms[i]);
		}
		IRenderable::mutexShaderProgram = shaderProgram;
	}

	if (enableAmbient)
		shaderProgram->setUniform("ambient", &ambient);
	if (enableDiffuse)
		shaderProgram->setUniform("diffuse", &diffuse);
	if (enableSpecular)
		shaderProgram->setUniform("specular", &specular);
	if (enableEmissive)
		shaderProgram->setUniform("emissive", &emissive);
	if (enableTransparencyFactor)
		shaderProgram->setUniform("transparencyFactor", transparencyFactor);
	if (enableShininessFactor)
		shaderProgram->setUniform("shininessFactor", shininessFactor);
	if (reflectionFactor)
		shaderProgram->setUniform("reflectionFactor", reflectionFactor);

	Texture* diffuseTexture = Resource::instance()->getTexture(diffuseTextureId);
	if (diffuseTexture != NULL) {
		diffuseTexture->bindTexture();
	}

	Texture* specularTexture = Resource::instance()->getTexture(specularTextureId);
	if (specularTexture != NULL) {
		specularTexture->bindTexture();
	}

	Texture* toonTexture = Resource::instance()->getTexture(toonTextureId);
	if (toonTexture != NULL) {
		toonTexture->bindTexture();
	}
}

