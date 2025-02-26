#pragma once

#include "vmath.hpp"

class Transform {
public:
  vec3 position;
  quat rotation;
  vec3 scale;
  Transform* parent;

  Transform();

  mat4 getLocalMatrix() const;

  mat4 getGlobalMatrix() const;

  quat getGlobalRotation() const;

  vec3 getGlobalPosition() const;

  vec3 getGlobalScale() const;

  void setRotation(const vec3& axis, float angle);

  void setRotationFromEuler(const vec3& eulerAngles);

  void rotate(const vec3& axis, float angle);

  vec3 forward() const;
  vec3 up() const;
  vec3 right() const;

  void setParent(Transform* newParent);

};


