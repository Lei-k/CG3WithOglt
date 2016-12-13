#pragma once

#include "oglt_util.h"
#include "oglt_shader.h"

namespace oglt {
	class IRenderable {
	public:
		virtual void render(int renderType = OGLT_RENDER_SELF) {}
		void setShaderProgram(ShaderProgram* shaderProgram){
			this->shaderProgram = shaderProgram;
		}
		ShaderProgram* getShaderProgram() {
			return shaderProgram;
		}

		static uint mutexShaderProgramId;
		static glm::mat4* mutexModelMatrix;
		static glm::mat4* mutexViewMatrix;
		static glm::mat4* mutexProjMatrix;
		static glm::mat4* mutexOrthoMatrix;
		static glm::vec3* mutexSunLightDir;
		static ShaderProgram* mutexShaderProgram;

	protected:
		bool visiable = true;
		ShaderProgram* shaderProgram;
	};
}