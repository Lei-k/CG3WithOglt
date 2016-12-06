#include "oglt_scene_node.h"

using namespace oglt::scene;

#include <glm/gtc/matrix_transform.hpp>

SceneNode::SceneNode()
{
	parent = NULL;
	child = NULL;
	brother = NULL;
	localTransform.position = glm::vec3(0.0f, 0.0f, 0.0f);
	localTransform.rotation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
	localTransform.scale = glm::vec3(1.0f, 1.0f, 1.0f);
	modelMatrix = glm::mat4(1);
}

SceneNode::~SceneNode()
{
	
}

Transform* SceneNode::getLocalTransform()
{
	return &localTransform;
}

Transform* SceneNode::getWorldTransform()
{
	return &worldTransform;
}

glm::mat4 * SceneNode::getModelMatrix()
{
	return &modelMatrix;
}

void SceneNode::calcNodeHeirarchyTransform()
{
	if (parent != NULL) {
		worldTransform.position = parent->worldTransform.position 
			+ parent->worldTransform.rotation * localTransform.position;
		worldTransform.rotation = parent->worldTransform.rotation * localTransform.rotation;
		worldTransform.scale = parent->worldTransform.scale * localTransform.scale;
	}
	else {
		worldTransform = localTransform;
	}
	glm::mat4 translateMatrix = glm::translate(glm::mat4(1), worldTransform.position);
	glm::mat4 rotationMatrix = glm::toMat4(worldTransform.rotation);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1), worldTransform.scale);
	modelMatrix = translateMatrix * rotationMatrix * scaleMatrix;
	
	if (brother != NULL)
		brother->calcNodeHeirarchyTransform();
	
	if (child != NULL)
		child->calcNodeHeirarchyTransform();
}

void SceneNode::addChild(SceneNode * child)
{
	if (this->child == NULL) {
		this->child = child;
	}
	else {
		SceneNode* lastBrother = this->child;
		while (lastBrother->brother != NULL) {
			lastBrother = lastBrother->brother;
		}
		lastBrother->brother = child;
	}
	child->parent = this;
}

void SceneNode::removeChild(SceneNode * child)
{
	SceneNode* node = this->child;
	while (node != child && node != NULL) {
		removeChild(node);
		node = child->brother;
	}
	if (node != NULL) {
		if (node->brother != NULL) {
			node->parent->child = node->brother;
		}
		if (node->child != NULL) {
			SceneNode* childNode = node->child;
			parent->addChild(childNode);
			SceneNode* brotherNode = childNode->brother;
			while (brotherNode != NULL) {
				brotherNode->parent = node->parent;
				brotherNode = brotherNode->brother;
			}
		}
	}
}

bool SceneNode::isRootNode()
{
	return parent == NULL;
}
