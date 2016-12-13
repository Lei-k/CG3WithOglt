#pragma once

#include "std_util.h"
#include "oglt_util.h"
#include "oglt_shader.h"
#include "oglt_texture.h"
#include "oglt_material.h"

namespace oglt {
	class Resource {
	public:
		static Resource* instance();

		void initialize();
		uint addShaderProgram(ShaderProgram& shaderProgram);
		ShaderProgram* getShaderProgram(uint shaderId);
		uint addTexture(Texture& texture);
		uint addTexture(const string& texturePath);
		Texture* getTexture(uint textureId);
		uint addMaterial(Material& material);
		Material* getMaterial(uint materialId);
		Material* findMaterial(const string& materialName);
	private:
#define DEFAULT_SHADER_PROGRAM_ID 0;
#define DEFAULT_TEXTURE_ID 0;
#define DEFAULT_MATERIAL_ID 0;

		Resource();
		~Resource();
		
		static Resource* resource;

		vector<ShaderProgram> shaderPrograms;
		vector<Texture> textures;
		vector<Material> materials;
		map<string, uint> materialMap;
	};
}