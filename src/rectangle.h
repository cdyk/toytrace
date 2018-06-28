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

class xz_rect : public intersectable
{
public:
  xz_rect() = delete;

  xz_rect(const vec2& p0, const vec2& p1, float offset, material* mat) : p0(p0), p1(p1), offset(offset), mat(mat) {}

  bool intersect(const ray& r, float t_min, float t_max, intersection& isec) const override;

  aabb bounding_box(float time0, float time1) const override;

  virtual float pdf_value(const vec3& origin, const vec3& dir) const override;

  virtual vec3 random(const vec3& origin) const override;

  material* mat;
  vec2 p0;
  vec2 p1;
  float offset;
};

class yz_rect : public intersectable
{
public:
  yz_rect() = delete;

  yz_rect(const vec2& p0, const vec2& p1, float offset, material* mat) : p0(p0), p1(p1), offset(offset), mat(mat) {}

  bool intersect(const ray& r, float t_min, float t_max, intersection& isec) const override;

  aabb bounding_box(float time0, float time1) const override;

  material* mat;
  vec2 p0;
  vec2 p1;
  float offset;
};
