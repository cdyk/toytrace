#pragma once
#include "vec3.h"

struct ray
{
  ray() {}
  ray(const vec3& o, const vec3& d) : o(o), d(d)
  {}


  const vec3& origin() const { return o; }
  const vec3& direction() const { return d; }

  vec3 at(const float t) { return o + t * d; }

  vec3 o;
  vec3 d;
 
};