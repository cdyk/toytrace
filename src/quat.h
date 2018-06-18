#pragma once
#include <cmath>
#include <limits>
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

quat conjugate(const quat& q)
{
  return quat(-q.x, -q.y, -q.z, q.w);
}

quat operator*(const quat& a, const quat& b)
{
  return quat(
    a.w * b.x + b.w * a.x + a.y * b.z - b.y * a.z,
    a.w * b.y + b.w * a.y + b.x * a.z - a.x * b.z,
    a.w * b.z + b.w * a.z + a.x * b.y - b.x * a.y,
    a.w * b.w - (a.x * b.x + a.y * b.y + a.z * b.z)
  );
}

vec3 transform(const quat & q, const vec3& v)
{
  //return (q*(quat(v, 0)*conjugate(q))).v;
  auto c = q.w;
  auto qq = q.x*q.x + q.y*q.y + q.z*q.z;
  auto vq = v.x*q.x + v.y*q.y + v.z*q.z;
  return (c*c - qq) * v + (2.f*vq)*q.v + (2.f*c)*cross(q.v, v);
}


quat normalize(const quat& q)
{
  auto l =
    q.data[0] * q.data[0] +
    q.data[1] * q.data[1] +
    q.data[2] * q.data[2] +
    q.data[3] * q.data[3];

  quat rv;
  if (std::numeric_limits<float>::epsilon() <= l) {
    auto s = 1.f / std::sqrt(l);
    rv.data[0] = s * q.data[0];
    rv.data[1] = s * q.data[1];
    rv.data[2] = s * q.data[2];
    rv.data[3] = s * q.data[3];
  }
  return rv;
}

// n assumed to be normalized
// positive angle is CCW when n points towards the viewer
quat axisAngle(const vec3& n, const float theta)
{
  const auto s = std::sin(0.5f * theta);
  const auto c = std::cos(0.5f * theta);

  return quat(s*vec3(n.x, n.y, n.z), c);
}

// a and b are assumed to be normalized
quat greatCircle(const vec3& a, const vec3& b)
{
  auto n = cross(a, b);
  auto l = dot(n, n);
  if (l < std::numeric_limits<float>::epsilon()) {
    return quat();
  }

  auto c = dot(a, b);
  auto s = sqrt((1.f - c * c) / l);
  return quat(s*n, c);
}

mat3 toMatrix(const quat& q)
{
  auto xx = q.data[0] * q.data[0];
  auto xy = q.data[0] * q.data[1];
  auto xz = q.data[0] * q.data[2];
  auto xw = q.data[0] * q.data[3];
  auto yy = q.data[1] * q.data[1];
  auto yz = q.data[1] * q.data[2];
  auto yw = q.data[1] * q.data[3];
  auto zz = q.data[2] * q.data[2];
  auto zw = q.data[2] * q.data[3];
  return mat3(1.f - 2.f*(yy + zz), 2.f*(xy + zw), 2.f*(xz - yw),
              2.f*(xy - zw), 1.f - 2.f*(xx + zz), 2.f*(yz + xw),
              2.f*(xz + yw), 2.f*(yz - xw), 1.f - 2.f*(xx + yy));
}