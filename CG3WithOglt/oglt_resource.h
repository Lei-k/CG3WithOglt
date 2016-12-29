#pragma once

#include "std_util.h"
#include "oglt_util.h"
#include "oglt_shader.h"
#include "oglt_texture.h"
#include "oglt_imaterial.h"
#include "oglt_skybox.h"
#include "oglt_ubo.h"
#include "oglt_light.h"

namespace oglt {
	class Resource {
	public:
		static Resource* instance();

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

		uint addDirectionalLight(const scene::DirectionalLight& directionalLight);
		uint addDirectionalLight(const scene::DirectionalLight& directionalLight, const string& lightName);
		uint addSpotLight(const scene::SpotLight& spotLight);
		uint addSpotLight(const scene::SpotLight& spotLight, const string& lightName);

		scene::DirectionalLight* getDirectionalLight(uint lightId);
		scene::DirectionalLight* findDirectionalLight(const string& lightName);
		scene::SpotLight* getSpotLight(uint lightId);
		scene::SpotLight* findSpotLight(const string& lightName);

		void setUpLights();
		void updateLights();
	private:
#define DEFAULT_SHADER_PROGRAM_ID 0;
#define DEFAULT_TEXTURE_ID 0;
#define DEFAULT_MATERIAL_ID 0;

#define MAX_DIRECTIONAL_LIGHTS 10;
#define MAX_SPOT_LIGHTS 10;

		void initialize();

		Resource();
		~Resource();

		vector<ShaderProgram> shaderPrograms;
		map<string, uint> shaderProgramMap;
		vector<Texture> textures;
		vector<IMaterial*> materials;
		map<string, uint> materialMap;
		vector<scene::Skybox> skyboxes;

		UniformBufferObject uboLights;
		vector<scene::DirectionalLight> directionalLights;
		map<string, uint> directionalLightMap;
		vector<scene::SpotLight> spotLights;
		map<string, uint> spotLightMap;
	};
}