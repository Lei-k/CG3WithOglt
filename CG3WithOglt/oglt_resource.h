#pragma once

#include "std_util.h"
#include "oglt_util.h"
#include "oglt_shader.h"
#include "oglt_texture.h"
#include "oglt_material.h"

namespace oglt {
	class Resource {
	public:
		static void initialize();
		static uint addShaderProgram(ShaderProgram& shaderProgram);
		static ShaderProgram* getShaderProgram(uint shaderId);
		static uint addTexture(Texture& texture);
		static Texture* getTexture(uint textureId);
		static uint addMaterial(Material& material);
		static Material* getMaterial(uint materialId);
	private:
#define DEFAULT_SHADER_PROGRAM_ID 0;
#define DEFAULT_TEXTURE_ID 0;
#define DEFAULT_MATERIAL_ID 0;

		static vector<ShaderProgram> shaderPrograms;
		static vector<Texture> textures;
		static vector<Material> materials;
	};
}