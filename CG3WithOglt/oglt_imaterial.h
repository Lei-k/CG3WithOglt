#pragma once

#include "std_util.h"
#include "oglt_util.h"
#include "oglt_texture.h"
#include "oglt_shader.h"
#include "oglt_ishaderable.h"
#include <glm/glm.hpp>

namespace oglt {
	class IMaterial : public IShaderable {
	public:
		virtual void useMaterial() {}

		void inline setName(const string& materialName) {
			name = materialName;
		}

		string inline getName() {
			return name;
		}
	protected:
		string name;
	};

	class IColorMaterial {
	public:
		void setColor(MaterialParam param, const glm::vec3& color) {}
		glm::vec3* getColor(MaterialParam param) { return nullptr; }
	};

	class IFactorMaterial {
	public:
		void setFactor(MaterialParam param, float factor) {}
		float getFactor(MaterialParam param) { return 0.0f; }
	};

	class ITexturedMaterial {
	public:
		void linkTexture(MaterialParam param, uint textureId) {}
		uint getTextureId(MaterialParam param) { return OGLT_INVALID_TEXTURE_ID; }
	};

	class ISkeletonMaterial : public IMaterial {
	public:
		void setBoneTransforms(vector<glm::mat4>& boneTransforms) {
			this->boneTransforms = boneTransforms;
		}
	protected:
		vector<glm::mat4> boneTransforms;
	};
}