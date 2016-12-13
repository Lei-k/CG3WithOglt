#pragma once

#include "oglt_imaterial.h"

namespace oglt {
	class SkeletonMaterial : public ISkeletonMaterial, IColorMaterial, IFactorMaterial
		, ITexturedMaterial {
	public:
		SkeletonMaterial();
		~SkeletonMaterial();

		void setColor(MaterialParam param, glm::vec3& color);
		void setFactor(MaterialParam param, float Factor);

		glm::vec3* getColor(MaterialParam param);
		float getFactor(MaterialParam param);
		void linkTexture(MaterialParam param, uint textureId);
		uint getTextureId(MaterialParam param);

		virtual void useMaterial();
	private:
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
	};
}