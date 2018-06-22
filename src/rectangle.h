#pragma once

#include "intersectable.h"
#include "vec2.h"

class xy_rect : public intersectable
{
public:
  xy_rect() = delete;

  xy_rect(const vec2& p0, const vec2& p1, float offset, material* mat) : p0(p0), p1(p1), offset(offset), mat(mat) {}

  bool intersect(const ray& r, float t_min, float t_max, intersection& isec) const override;

  aabb bounding_box(float time0, float time1) const override;

  material* mat;
  vec2 p0;
  vec2 p1;
  float offset;
};