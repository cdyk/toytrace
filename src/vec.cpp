#include <limits>
#include <cmath>

#include "vec2.h"
#include "vec3.h"
#include "vec4.h"

inline float length(const vec2& v)
{
  return std::sqrt(v.x*v.x + v.y*v.y);
}

inline vec2 normalize(const vec2& x) {
  auto l = dot(x, x);
  auto s = l < std::numeric_limits<float>::epsilon() ? 0.f : 1.f / std::sqrt(l);
  return s * x;
}


float length(const vec3& v)
{
  return std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

vec3 normalize(const vec3& x) {
  auto l = dot(x, x);
  auto s = l < std::numeric_limits<float>::epsilon() ? 0.f : 1.f / std::sqrt(l);
  return s * x;
}
