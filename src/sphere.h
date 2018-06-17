#pragma once

#include "intersectable.h"

class sphere : public intersectable
{
public:

  sphere() {}

  sphere(const vec3& center, float radius) : center(center), radius(radius) {}

  virtual bool intersect(const ray& r, float t_min, float t_max, intersection& rec) const override;

  vec3 center;
  float radius;

};