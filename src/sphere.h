#pragma once

#include "intersectable.h"

class material;

class sphere : public intersectable
{
public:

  sphere() {}

  sphere(const vec3& center, float radius, material* mat) : center(center), radius(radius), mat(mat) {}

  virtual bool intersect(const ray& r, float t_min, float t_max, intersection& rec) const override;

  vec3 center;
  float radius;
  material * mat;
};

class moving_sphere : public intersectable
{
public:

  moving_sphere() {}

  moving_sphere(const vec3& center0, const vec3& center1, float time0, float time1, float radius, material* mat)
    : center0(center0), center1(center1), time0(time0), time1(time1), radius(radius), mat(mat) {}

  virtual bool intersect(const ray& r, float t_min, float t_max, intersection& rec) const override;

  vec3 center0;
  vec3 center1;
  float time0;
  float time1;
  float radius;
  material * mat;
};

