#include "oglt_ubo.h"

using namespace oglt;

UniformBufferObject::UniformBufferObject()
{

}
UniformBufferObject::~UniformBufferObject()
{

}

void UniformBufferObject::createUniformBuffer()
{
	glGenBuffers(1, &bufferId);
}

void printMat4(glm::mat4* mat) {
	FOR(i, 4) {
		FOR(j, 4) {
			cout << (*mat)[i][j] << "\t";
		}
		cout << endl;
	}
	cout << endl;
}

void UniformBufferObject::updateBuffer()
{
	glBindBuffer(GL_UNIFORM_BUFFER, bufferId);
	GLuint from = 0;

	FOR(i, ESZ(datas)) {
		if (datas[i] != NULL) {
			glBufferSubData(GL_UNIFORM_BUFFER, from, sizes[i], datas[i]);
		}
		from += sizes[i];
	}
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}



void UniformBufferObject::addData(void* data, GLuint size) {
	datas.push_back(data);
	sizes.push_back(size);
	dataSize += size;
}

GLuint UniformBufferObject::getDataSize()
{
	return dataSize;
}

void UniformBufferObject::uploadBufferData(GLuint bindingIndex, GLenum mode) {

	glBindBuffer(GL_UNIFORM_BUFFER, bufferId);
	glBufferData(GL_UNIFORM_BUFFER, dataSize, NULL, mode);
	glBindBufferRange(GL_UNIFORM_BUFFER, bindingIndex, bufferId, 0, dataSize);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBufferObject::deleteBuffer()
{
	glDeleteBuffers(1, &bufferId);
}


