#pragma once
#include "vec3.h"

struct ray
{
  ray() {}
  ray(const vec3& ori, const vec3& dir, float time) : ori(ori), dir(dir), time(time)
  {}


  vec3 at(const float t) const { return ori + t * dir; }

  vec3 ori;
  vec3 dir;
  float time;
};