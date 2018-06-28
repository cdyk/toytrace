#pragma once
#include "ray.h"
#include "texture.h"

struct intersection;

class pdf;

struct ScatterData
{
  ray specularRay;
  pdf* pdf;
  vec3 attenuation;
  bool isSpecular;
};

class material
{
public:

  virtual bool scatter(ScatterData& scatterData,
                       const ray& r_in,
                       const intersection& hit) const = 0;

  // Probability of scattering in r_scattered direction.
  virtual float scattering_pdf(const ray& r_in, const intersection& hit, const ray& r_scattered) const = 0;

  virtual vec3 emitted(const ray& r_in, const intersection& hitData, const vec2& t, const vec3& p) const { return vec3(0); }
};

class lambertian : public material
{
public:
  lambertian() = delete;

  lambertian(texture* albedo) : albedo(albedo) {}

  virtual bool scatter(ScatterData& scatterData,
                       const ray& r_in,
                       const intersection& hit) const override;

  virtual float scattering_pdf(const ray& r_in, const intersection& hit, const ray& r_scattered) const override;

  texture* albedo;
};

class metal : public material
{
public:
  metal(const vec3& albedo, float fuzz) : albedo(albedo), fuzz(fuzz <= 1.f ? fuzz : 1.f) {}

  virtual bool scatter(ScatterData& scatterData,
                       const ray& r_in,
                       const intersection& hit) const override;

  virtual float scattering_pdf(const ray& r_in, const intersection& hit, const ray& r_scattered) const  override { return 0.f; }

  vec3 albedo;
  float fuzz;

};

#if 0

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

#endif

class diffuse_light : public material
{
public:
  diffuse_light() = delete;

  diffuse_light(texture* emit) : emit(emit) {}

  virtual bool scatter(ScatterData& scatterData,
                       const ray& r_in,
                       const intersection& hit) const override { return false; }
  virtual float scattering_pdf(const ray& r_in, const intersection& hit, const ray& r_scattered) const override { return 0.f; }

  virtual vec3 emitted(const ray& r_in, const intersection& hitData, const vec2& t, const vec3& p) const override;
  
  texture* emit;
};

#if 0
class isotropic : public material
{
public:
  isotropic() = delete;

  isotropic(texture* albedo) : albedo(albedo) {}

  virtual bool scatter(ray& scattered,
                       vec3& attenuation,
                       const ray& r_in,
                       const intersection& hit) const override;

  texture* albedo;
};
#endif
