#pragma once
#include "ray.h"
#include <cfloat>

struct aabb
{
  aabb() : min(FLT_MAX), max(-FLT_MAX) {}

  aabb(const vec3& min, const vec3& max) : min(min), max(max) {}

  bool empty() const { return max.x < min.x; }

  void include(const aabb& box)
  {
    for (unsigned a = 0; a < 3; a++) {
      min[a] = min[a] < box.min[a] ? min[a] : box.min[a];
      max[a] = box.max[a] < max[a] ? max[a] : box.max[a];
    }
  }

  void include(const vec3& p)
  {
    for (unsigned a = 0; a < 3; a++) {
      min[a] = min[a] < p[a] ? min[a] : p[a];
      max[a] = p[a] < max[a] ? max[a] : p[a];
    }
  }

  bool intersects(const ray& r, float t_min, float t_max) const
  {
    for (unsigned a = 0; a < 3; a++) {
      auto rcp = 1.f / r.dir[a];
      auto t0 = rcp * (min[a] - r.ori[a]);
      auto t1 = rcp * (max[a] - r.ori[a]);
      if (rcp < 0.f) {
        auto t = t0; t0 = t1; t1 = t;
      }
      t_min = t0 > t_min ? t0 : t_min;
      t_max = t1 < t_max ? t1 : t_max;
      if (t_max <= t_min) return false;
    }
    return true;
  }

  vec3 min;
  vec3 max;
};