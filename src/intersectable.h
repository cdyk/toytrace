#pragma once

#include "ray.h"
#include "aabb.h"
#include <vector>

class material;

struct intersection
{
  float t;
  vec3 p;
  vec3 n;
  material* mat;
};

class intersectable
{
public:
  virtual bool intersect(const ray& r, float t_min, float t_max, intersection& isec) const = 0;

  virtual aabb bounding_box(float time0, float time1) const = 0;
};

class intersectable_container : public intersectable
{
public:
  intersectable_container() {}

  virtual bool intersect(const ray& r, float t_min, float t_max, intersection& isec) const override;

  virtual aabb bounding_box(float time0, float time1) const override;

  std::vector<intersectable*> items;
};