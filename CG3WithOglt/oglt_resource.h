#pragma once

#include "std_util.h"
#include "oglt_util.h"
#include "oglt_shader.h"
#include "oglt_texture.h"
#include "oglt_imaterial.h"
#include "oglt_skybox.h"

namespace oglt {
	class Resource {
	public:
		static Resource* instance();

		void initialize();
		uint addShaderProgram(ShaderProgram& shaderProgram);
		uint addShaderProgram(ShaderProgram& shaderProgram, const string& programName);
		ShaderProgram* getShaderProgram(uint shaderId);
		ShaderProgram* findShaderProgram(const string& programName);
		uint addTexture(Texture& texture);
		uint addTexture(const string& texturePath);
		Texture* getTexture(uint textureId);
		uint addMaterial(IMaterial* material);
		IMaterial* getMaterial(uint materialId);
		IMaterial* findMaterial(const string& materialName);
		uint addSkybox(const scene::Skybox& skybox);
		scene::Skybox* getSkybox(uint skyboxId);
	private:
#define DEFAULT_SHADER_PROGRAM_ID 0;
#define DEFAULT_TEXTURE_ID 0;
#define DEFAULT_MATERIAL_ID 0;

		Resource();
		~Resource();

		vector<ShaderProgram> shaderPrograms;
		map<string, uint> shaderProgramMap;
		vector<Texture> textures;
		vector<IMaterial*> materials;
		map<string, uint> materialMap;
		vector<scene::Skybox> skyboxes;
	};
}