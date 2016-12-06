#include "oglt_material.h"

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

	diffuseTextureIndex = OGLT_INVALID_TEXTURE_INDEX;
	specularTextureIndex = OGLT_INVALID_TEXTURE_INDEX;
	toonTextureIndex = OGLT_INVALID_TEXTURE_INDEX;
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
		diffuseTextureIndex = textureId;
		break;
	case SPECULAR:
		specularTextureIndex = textureId;
		break;
	case TOON:
		toonTextureIndex = textureId;
		break;
	default:
		fprintf(stderr, "Error: Use undefined texture param\n");
	}
}

uint oglt::Material::getTextureIndex(MaterialParam param)
{
	switch (param) {
	case DIFFUSE:
		return diffuseTextureIndex;
	case SPECULAR:
		return specularTextureIndex;
	case TOON:
		return toonTextureIndex;
	default:
		fprintf(stderr, "Error: Use undefined texture param\n");
	}
	return OGLT_INVALID_TEXTURE_INDEX;
}


void oglt::Material::setShaderProgram(ShaderProgram * shaderProgram)
{
	this->shaderProgram = shaderProgram;
}

void oglt::Material::useMaterial()
{
	if (shaderProgram == NULL)
		return;

	//shaderProgram->useProgram();
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
}

