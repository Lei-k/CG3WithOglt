#pragma once

#include "oglt_shader.h"

namespace oglt {
	class IShaderable {
	public:
		inline void setShaderProgram(ShaderProgram* _shaderProgram) {
			shaderProgram = _shaderProgram;
		}

		inline ShaderProgram* getShaderProgram() {
			return shaderProgram;
		}
	protected:
		ShaderProgram* shaderProgram;
	};
}