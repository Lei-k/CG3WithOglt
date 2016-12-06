#include "oglt_resource.h"

using namespace oglt;

vector<ShaderProgram> Resource::shaderPrograms;
vector<Texture> Resource::textures;
vector<Material> Resource::materials;

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
		shaderPrograms.push_back(shaderProgram);
		shaderProgramId = ESZ(shaderPrograms) - 1;
	}

	return shaderProgramId;
}

ShaderProgram * Resource::getShaderProgram(uint shaderId)
{
	if (shaderId == OGLT_INVALID_SHADER_ID || shaderId >= ESZ(shaderPrograms)) {
		// if use invalid shader programId set to default shader program id
		shaderId = DEFAULT_SHADER_PROGRAM_ID;
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
		textures.push_back(texture);
		textureId = ESZ(textures) - 1;
	}
	return textureId;
}

Texture * Resource::getTexture(uint textureId)
{
	if (textureId == OGLT_INVALID_TEXTURE_ID || textureId >= ESZ(textures)) {
		return NULL;
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
		materials.push_back(material);
		materialId = ESZ(materials) - 1;
	}
	return materialId;
}

Material * Resource::getMaterial(uint materialId)
{
	if (materialId == OGLT_INVALID_MATERIAL_ID || materialId >= ESZ(materials)) {
		materialId = DEFAULT_MATERIAL_ID;
	}
	return &materials[materialId];
}
