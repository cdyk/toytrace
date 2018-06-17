#pragma once
#include <cmath>
#include <limits>
#include "vec3.h"
#include "mat3.h"

struct quat
{
  quat() { data[0] = 1.f; data[1] = 0.f; data[1] = 0.f; data[2] = 0.f; }
  quat(const vec3& v, const float w) { data[0] = v.x;  data[1] = v.y;  data[2] = v.z; data[3] = w; }

  float data[4];
};

quat operator*(const quat& a, const quat& b)
{
  return quat(
    vec3(a.data[3] * b.data[0] + b.data[3] * a.data[0] + a.data[1] * b.data[2] - b.data[1] * a.data[2],
         a.data[3] * b.data[1] + b.data[3] * a.data[1] + b.data[0] * a.data[2] - a.data[0] * b.data[2],
         a.data[3] * b.data[2] + b.data[3] * a.data[2] + a.data[0] * b.data[1] - b.data[0] * a.data[1]),
    a.data[3] * b.data[3] - (a.data[0] * b.data[0] + a.data[1] * b.data[1] + a.data[2] * b.data[2])
  );
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