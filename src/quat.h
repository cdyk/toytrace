#pragma once
#include "vec3.h"
#include "mat3.h"

struct quat
{
  quat() : x(0), y(0), z(0), w(1) { }
  quat(const vec3& v, const float w) : v(v), s(w) { }
  quat(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
  union {
    float data[4];
    struct {
      vec3 v;
      float s;
    };
    struct {
      float x;
      float y;
      float z;
      float w;
    };

  };
};

inline quat conjugate(const quat& q)
{
  return quat(-q.x, -q.y, -q.z, q.w);
}

quat operator*(const quat& a, const quat& b);

vec3 transform(const quat & q, const vec3& v);

quat normalize(const quat& q);

// n assumed to be normalized
// positive angle is CCW when n points towards the viewer
quat axisAngle(const vec3& n, const float theta);

// a and b are assumed to be normalized
quat greatCircle(const vec3& a, const vec3& b);

mat3 toMatrix(const quat& q);
