#pragma once

#include "intersectable.h"

class bvh : public intersectable
{
public:
  bvh(const std::vector<intersectable*>& items, float t0, float  t1, unsigned axis=0);

  virtual bool intersect(const ray& r, float t_min, float t_max, intersection& isec) const override;

  virtual aabb bounding_box(float time0, float time1) const override;

  intersectable* children[2] = { nullptr, nullptr };
  aabb bbox;
};