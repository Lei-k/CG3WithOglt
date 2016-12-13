#pragma once

#include "std_util.h"
#include "oglt_texture.h"
#include "oglt_shader.h"
#include <glm/glm.hpp>

namespace oglt {
	enum MaterialParam {
		AMBIENT, DIFFUSE, SPECULAR, EMISSIVE,
		TRANSPARENCY_FACTOR, SHININESS_FACTOR, REFLECTION_FACTOR,
		TOON
	};

	class Material {
	public:
		Material();
		~Material();

		void setColorParam(MaterialParam param, glm::vec3& color);
		void setFactorParam(MaterialParam param, float Factor);

		glm::vec3* getColorParam(MaterialParam param);
		float getFactorParam(MaterialParam param);
		void linkTexture(MaterialParam param, uint textureId);
		uint getTextureId(MaterialParam param);
		void setShaderProgram(ShaderProgram* shaderProgram);
		ShaderProgram* getShaderProgram();

		void setBoneTransforms(const vector<glm::mat4>& boneTransforms);
		void setName(const string& materialName);
		const string& getName();

		void useMaterial();
	private:
		string materialName;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		glm::vec3 emissive;
		float transparencyFactor;
		float shininessFactor;
		float reflectionFactor;

		bool enableAmbient;
		bool enableDiffuse;
		bool enableSpecular;
		bool enableEmissive;
		bool enableTransparencyFactor;
		bool enableShininessFactor;
		bool enableReflectionFactor;

		uint diffuseTextureId;
		uint specularTextureId;
		uint toonTextureId;

		vector<glm::mat4> boneTransforms;

		ShaderProgram* shaderProgram;
	};
}