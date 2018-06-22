#pragma once
#include "ray.h"
#include "texture.h"

struct intersection;

class material
{
public:

  virtual bool scatter(ray& scattered,
                       vec3& attenuation,
                       const ray& r_in,
                       const intersection& hit) const = 0;

  virtual vec3 emitted(const vec2& t, const vec3& p) const { return vec3(0); }
};

class lambertian : public material
{
public:
  lambertian() = delete;

  lambertian(texture* albedo) : albedo(albedo) {}

  virtual bool scatter(ray& scattered,
                       vec3& attenuation,
                       const ray& r_in,
                       const intersection& hit) const override;

  texture* albedo;
};

class metal : public material
{
public:
  metal(const vec3& albedo, float fuzz) : albedo(albedo), fuzz(fuzz <= 1.f ? fuzz : 1.f) {}

  virtual bool scatter(ray& scattered,
                       vec3& attenuation,
                       const ray& r_in,
                       const intersection& hit) const override;

  vec3 albedo;
  float fuzz;

};

class dielectric : public material
{
public:
  dielectric(float ref_idx) : ref_idx(ref_idx) {}

  virtual bool scatter(ray& scattered,
                       vec3& attenuation,
                       const ray& r_in,
                       const intersection& hit) const override;

  float ref_idx;
};

class diffuse_light : public material
{
public:
  diffuse_light() = delete;

  diffuse_light(texture* emit) : emit(emit) {}

  virtual bool scatter(ray& scattered,
                       vec3& attenuation,
                       const ray& r_in,
                       const intersection& hit) const override { return false; }

  virtual vec3 emitted(const vec2& t, const vec3& p) const { return emit->value(t, p); }

  texture* emit;
};