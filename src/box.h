#pragma once

#include "intersectable.h"

class box : public intersectable
{
public:
  box() = delete;

  box(const vec3& p0, const vec3& p1, material* mat);

  virtual bool intersect(const ray& r, float t_min, float t_max, intersection& isec) const override { return walls.intersect(r, t_min, t_max, isec); }

  virtual aabb bounding_box(float time0, float time1) const override { return aabb(p0, p1); }

  material* mat;
  intersectable_container walls;
  vec3 p0;
  vec3 p1;
};