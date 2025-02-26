#include "../transform.hpp"

Transform::Transform()
: position(0.0f), rotation(0.0f, 0.0f, 0.0f, 1.0f), scale(1.0f), parent(nullptr) 
{}

mat4 Transform::getLocalMatrix() const {
	mat4 translationMatrix = mat4::translation(position);
	mat4 rotationMatrix = rotation.toMat4();
	mat4 scaleMatrix = mat4::scale(scale);

	return translationMatrix * rotationMatrix * scaleMatrix;
}

quat Transform::getGlobalRotation() const {
	quat globalRotation = rotation;
	Transform* currentParent = parent;

	while (currentParent) {
		globalRotation = currentParent->rotation * globalRotation;
		currentParent = currentParent->parent;
	}

	return globalRotation;
}

mat4 Transform::getGlobalMatrix() const {
	mat4 globalMatrix = getLocalMatrix();
	Transform* currentParent = parent;

	while (currentParent) {
		globalMatrix = currentParent->getLocalMatrix() * globalMatrix;
		currentParent = currentParent->parent;
	}

	return globalMatrix;
}

vec3 Transform::getGlobalScale() const {
	vec3 globalScale = scale;
	Transform* currentParent = parent;

	while (currentParent) {
		globalScale.x *= currentParent->scale.x;
		globalScale.y *= currentParent->scale.y;
		globalScale.z *= currentParent->scale.z;
		currentParent = currentParent->parent;
	}

	return globalScale;
}

vec3 Transform::getGlobalPosition() const {
	vec3 globalPosition = position;
	Transform* currentParent = parent;

	while (currentParent) {
		globalPosition = currentParent->position + (currentParent->rotation * (currentParent->scale * globalPosition));
		currentParent = currentParent->parent;
	}

	return globalPosition;
}

void Transform::setRotation(const vec3& axis, float angle) {
	rotation = quat::fromAxisAngle(axis, angle);
}

void Transform::setRotationFromEuler(const vec3& eulerAngles) {
	float yaw   = eulerAngles.y;   // rotation around y-axis
	float pitch = eulerAngles.x; // rotation around x-axis
	float roll  = eulerAngles.z;  // rotation around z-axis

	float cy = cosf(yaw * 0.5f);    
	float sy = sinf(yaw * 0.5f);    
	float cp = cosf(pitch * 0.5f);  
	float sp = sinf(pitch * 0.5f);  
	float cr = cosf(roll * 0.5f);   
	float sr = sinf(roll * 0.5f);   

	rotation.x = cy * sp * cr + sy * cp * sr;
	rotation.y = sy * cp * cr - cy * sp * sr;
	rotation.z = cy * cp * sr - sy * sp * cr;
	rotation.w = cy * cp * cr + sy * sp * sr;
}

void Transform::rotate(const vec3& axis, float angle) {
	quat incrementalRotation = quat::fromAxisAngle(axis, angle);
	rotation = (incrementalRotation * rotation).normalize();
}

vec3 Transform::forward() const {
	return getGlobalRotation() * vec3(0.0f, 0.0f, -1.0f);
}

vec3 Transform::up() const {
	return getGlobalRotation() * vec3(0.0f, 1.0f, 0.0f);
}

vec3 Transform::right() const {
	return getGlobalRotation() * vec3(1.0f, 0.0f, 0.0f);
}

void Transform::setParent(Transform* newParent) {
	parent = newParent;
}

