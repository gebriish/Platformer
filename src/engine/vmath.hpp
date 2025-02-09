#pragma once
#include <cmath>
#include <cstring>

constexpr float VMATH_PI   =    3.14159265358979323846;  /* pi   */
constexpr float VMATH_PI_2 =		1.57079632679489661923;	 /* pi/2 */
constexpr float VMATH_PI_4 =		0.78539816339744830962;	 /* pi/4 */

namespace vmath {

//############################################################//
//#                         vec2                             #//
//############################################################//

struct vec2 {
  vec2():
    x(0), y(0)
  {}

  vec2(float d)
  : x(d), y(d)
  {}

  vec2(float v1, float vec2)
  : x(v1), y(vec2)
  {}

  union {
    struct { float x, y; };
    float xy[2];
  };

  inline bool operator==(const vec2& other) {
    return x == other.x && y == other.y;
  }

  inline vec2 operator+(float s) const { return vec2(x + s, y + s); }
  inline vec2 operator-(float s) const { return vec2(x - s, y - s); }
  inline vec2 operator*(float s) const { return vec2(x * s, y * s); }
  inline vec2 operator/(float s) const { return vec2(x / s, y / s); }

  inline vec2 operator+(const vec2& v) const { return vec2(x + v.x, y + v.y); }
  inline vec2 operator-(const vec2& v) const { return vec2(x - v.x, y - v.y); }
  inline vec2 operator*(const vec2& v) const { return vec2(x * v.x, y * v.y); }
  inline vec2 operator/(const vec2& v) const { return vec2(x / v.x, y / v.y); }

  inline vec2& operator+=(float s) { x += s; y += s; return *this; }
  inline vec2& operator-=(float s) { x -= s; y -= s; return *this; }
  inline vec2& operator*=(float s) { x *= s; y *= s; return *this; }
  inline vec2& operator/=(float s) { x /= s; y /= s; return *this; }

  inline vec2& operator+=(const vec2& v) { x += v.x; y += v.y; return *this; }
  inline vec2& operator-=(const vec2& v) { x -= v.x; y -= v.y; return *this; }
  inline vec2& operator*=(const vec2& v) { x *= v.x; y *= v.y; return *this; }
  inline vec2& operator/=(const vec2& v) { x /= v.x; y /= v.y; return *this; }
};

//############################################################//
//#                         vec3                             #//
//############################################################//

struct vec3 {
  vec3()
  : x(0), y(0), z(0)
  {}

  vec3(float d)
  : x(d), y(d), z(d)
  {}

  vec3(float v1, float vec2, float vec3)
  : x(v1), y(vec2), z(vec3)
  {}

  union {
    struct { float x, y, z; };
    float xyz[3];
  };

  inline bool operator==(const vec3& other) { 
    return x == other.x && y == other.y && z == other.z; 
  }

  inline vec3 operator+(float s) const { return vec3(x + s, y + s, z + s); }
  inline vec3 operator-(float s) const { return vec3(x - s, y - s, z - s); }
  inline vec3 operator*(float s) const { return vec3(x * s, y * s, z * s); }
  inline vec3 operator/(float s) const { return vec3(x / s, y / s, z / s); }

  inline vec3 operator+(const vec3& v) const { return vec3(x + v.x, y + v.y, z + v.z); }
  inline vec3 operator-(const vec3& v) const { return vec3(x - v.x, y - v.y, z - v.z); }
  inline vec3 operator*(const vec3& v) const { return vec3(x * v.x, y * v.y, z * v.z); }
  inline vec3 operator/(const vec3& v) const { return vec3(x / v.x, y / v.y, z / v.z); }

  inline vec3& operator+=(float s) { x += s; y += s; z += s; return *this; }
  inline vec3& operator-=(float s) { x -= s; y -= s; z -= s; return *this; }
  inline vec3& operator*=(float s) { x *= s; y *= s; z *= s; return *this; }
  inline vec3& operator/=(float s) { x /= s; y /= s; z /= s; return *this; }

  inline vec3& operator+=(const vec3& v) { x += v.x; y += v.y; z += v.z; return *this; }
  inline vec3& operator-=(const vec3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
  inline vec3& operator*=(const vec3& v) { x *= v.x; y *= v.y; z *= v.z; return *this; }
  inline vec3& operator/=(const vec3& v) { x /= v.x; y /= v.y; z /= v.z; return *this; }
};


//############################################################//
//#                         vec4                             #//
//############################################################//

struct vec4 {
  vec4()
  : x(0), y(0), z(0), w(0)
  {}

  vec4(float d)
  : x(d), y(d), z(d), w(d)
  {}

  vec4(float v1, float vec2, float vec3, float vec4)
  : x(v1), y(vec2), z(vec3), w(vec4)
  {}

  union {
    struct { float x, y, z, w; };
    float xyzw[4];
  };

  inline bool operator==(const vec4& other) { 
    return x == other.x && y == other.y && z == other.z && w == other.w; 
  }

  inline vec4 operator+(float s) const { return vec4(x + s, y + s, z + s, w + s); }
  inline vec4 operator-(float s) const { return vec4(x - s, y - s, z - s, w - s); }
  inline vec4 operator*(float s) const { return vec4(x * s, y * s, z * s, w * s); }
  inline vec4 operator/(float s) const { return vec4(x / s, y / s, z / s, w / s); }

  inline vec4 operator+(const vec4& v) const { return vec4(x + v.x, y + v.y, z + v.z, w + v.w); }
  inline vec4 operator-(const vec4& v) const { return vec4(x - v.x, y - v.y, z - v.z, w - v.w); }
  inline vec4 operator*(const vec4& v) const { return vec4(x * v.x, y * v.y, z * v.z, w * v.w); }
  inline vec4 operator/(const vec4& v) const { return vec4(x / v.x, y / v.y, z / v.z, w / v.w); }

  inline vec4& operator+=(float s) { x += s; y += s; z += s; w += s; return *this; }
  inline vec4& operator-=(float s) { x -= s; y -= s; z -= s; w -= s; return *this; }
  inline vec4& operator*=(float s) { x *= s; y *= s; z *= s; w *= s; return *this; }
  inline vec4& operator/=(float s) { x /= s; y /= s; z /= s; w /= s; return *this; }

  inline vec4& operator+=(const vec4& v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
  inline vec4& operator-=(const vec4& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
  inline vec4& operator*=(const vec4& v) { x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this; }
  inline vec4& operator/=(const vec4& v) { x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this; }
};

//############################################################//
//                       utility                              //
//############################################################//

inline float clamp(float min, float max, float val) {
  if(val < min)
    return min;
  else if(val > max)
    return max;
  else 
    return val;
}

inline vec2 clamp(const vec2& min, const vec2& max, const vec2& vec) {
  vec2 result;
  result.x = clamp(min.x, max.x, vec.x);
  result.y = clamp(min.y, max.y, vec.y);
  return result;
}

inline vec3 clamp(const vec3& min, const vec3& max, const vec3& vec) {
  vec3 result;
  result.x = clamp(min.x, max.x, vec.x);
  result.y = clamp(min.y, max.y, vec.y);
  result.z = clamp(min.z, max.z, vec.z);
  return result;
}

inline vec4 clamp(const vec4& min, const vec4& max, const vec4& vec) {
  vec4 result;
  result.x = clamp(min.x, max.x, vec.x);
  result.y = clamp(min.y, max.y, vec.y);
  result.z = clamp(min.z, max.z, vec.z);
  result.w = clamp(min.w, max.w, vec.w);
  return result;
}

inline float min(float a, float b) {
  return (a < b) ? a : b;
}

inline vec2 min(const vec2& a, const vec2& b) {
  return vec2(min(a.x, b.x), min(a.y, b.y));
}

inline vec3 min(const vec3& a, const vec3& b) {
  return vec3(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z));
}

inline vec4 min(const vec4& a, const vec4& b) {
  return vec4(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z), min(a.w, b.w));
}

inline float max(float a, float b) {
  return (a > b) ? a : b;
}

inline vec2 max(const vec2& a, const vec2& b) {
  return vec2(max(a.x, b.x), max(a.y, b.y));
}

inline vec3 max(const vec3& a, const vec3& b) {
  return vec3(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z));
}

inline vec4 max(const vec4& a, const vec4& b) {
  return vec4(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z), max(a.w, b.w));
}

inline float lerp(float a, float b, float t) {
  return a + (b - a) * t;
}

inline vec2 lerp(const vec2& a, const vec2& b, float t) {
  return a + (b - a) * t;
}

inline vec3 lerp(const vec3& a, const vec3& b, float t) {
  return a + (b - a) * t;
}

inline vec4 lerp(const vec4& a, const vec4& b, float t) {
  return a + (b - a) * t;
}

inline vec2 vec2_rotate(const vec2& v, float radians) {
  const float sinValue = sin(radians);
  const float cosValue = cos(radians);

  return vec2(v.x * cosValue - v.y * sinValue, v.x * sinValue + v.y * cosValue);
}

inline float dot(const vec2& a, const vec2& b) {
  return a.x * b.x + a.y * b.y;
}

inline float dot(const vec3& a, const vec3& b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline float dot(const vec4& a, const vec4& b) {
  return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

inline float length(const vec2& v) {
  return sqrt(dot(v, v));
}

inline float length(const vec3& v) {
  return sqrt(dot(v, v));
}

inline float length(const vec4& v) {
  return sqrt(dot(v, v));
}

inline vec2 normalize(const vec2& v) {
  const float l = length(v);
  if(l == 0.0f) {
    return vec2(0.0f);
  }

  return v / l;
}

inline vec3 normalize(const vec3& v) {
  const float l = length(v);
  if(l == 0.0f) {
    return vec3(0.0f);
  }

  return v / l;
}

inline vec4 normalize(const vec4& v) {
  const float l = length(v);
  if(l == 0.0f) {
    return vec4(0.0f);
  }

  return v / l;
}

inline float cross(const vec2& a, const vec2& b) {
  return a.x * b.y - a.y * b.x;
}

inline vec3 cross(const vec3& a, const vec3& b) {
  return vec3(
    a.y * b.z - a.z * b.y,
    a.z * b.x - a.x * b.z,
    a.x * b.y - a.y * b.x
  );
}

inline vec2 reflect(const vec2& ray, const vec2& normal) {
  const float component = dot(ray, normal);
  return ray - normal * component * 2.0f;
}

inline vec3 reflect(const vec3& ray, const vec3& normal) {
  const float component = dot(ray, normal);
  return ray - normal * component * 2.0f;
}

inline vec4 reflect(const vec4& ray, const vec4& normal) {
  const float component = dot(ray, normal);
  return ray - normal * component * 2.0f;
}

} // namespace vmath
