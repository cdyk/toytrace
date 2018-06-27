#pragma once

#include "ray.h"
#include "vec2.h"
#include "aabb.h"
#include <vector>

class material;
class texture;

struct intersection
{
  float t;
  vec2 u;
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

class normal_flip : public intersectable
{
public:
  normal_flip() = delete;

  normal_flip(intersectable * obj) : obj(obj) {}

  virtual bool intersect(const ray& r, float t_min, float t_max, intersection& isec) const override
  {
    if (obj->intersect(r, t_min, t_max, isec)) {
      isec.n = -isec.n;
      return true;
    }
    return false;
  }

  virtual aabb bounding_box(float time0, float time1) const override { return obj->bounding_box(time0, time1); }

  intersectable * obj;
};

class translate : public intersectable
{
public:
  translate() = delete;

  translate(intersectable* obj, const vec3& shift) : obj(obj), shift(shift) {}

  virtual bool intersect(const ray& r, float t_min, float t_max, intersection& isec) const override;

  virtual aabb bounding_box(float time0, float time1) const override;

  intersectable * obj;
  vec3 shift;
};

class rotate_y : public intersectable
{
public:
  rotate_y(intersectable* obj, float theta);

  virtual bool intersect(const ray& r, float t_min, float t_max, intersection& isec) const override;

  virtual aabb bounding_box(float time0, float time1) const override;

  intersectable * obj;
  float theta_cos;
  float theta_sin;
};

#if 0
class constant_medium : public intersectable
{
public:
  constant_medium() = delete;

  constant_medium(intersectable* boundary, float density, texture* phase_func);

  virtual bool intersect(const ray& r, float t_min, float t_max, intersection& isec) const override;

  virtual aabb bounding_box(float time0, float time1) const override { return boundary->bounding_box(time0, time1); }

  intersectable* boundary;
  float density;
  material* phase_func;
};
#endif