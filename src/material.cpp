#include "funcs.h"
#include "material.h"
#include "intersectable.h"

bool lambertian::scatter(ray& scattered,
                         vec3& attenuation,
                         const ray& r_in,
                         const intersection& hit) const
{
  vec3 target = hit.p + hit.n + random_in_unit_sphere();
  scattered = ray(hit.p, target - hit.p, r_in.time);
  attenuation = albedo->value(hit.u, hit.p);
  return true;
}


bool metal::scatter(ray& scattered,
                    vec3& attenuation,
                    const ray& r_in,
                    const intersection& hit) const
{
  scattered = ray(hit.p, reflect(normalize(r_in.dir), hit.n) + fuzz*random_in_unit_sphere(), r_in.time);
  attenuation = albedo;
  return 0 < dot(scattered.dir, hit.n);
}


bool dielectric::scatter(ray& scattered,
                         vec3& attenuation,
                         const ray& r_in,
                         const intersection& hit) const
{
  vec3 outward_n;
  float ni_over_nt;
  float cosine;
  if (0 < dot(r_in.dir, hit.n)) {
    outward_n = -hit.n;
    ni_over_nt = ref_idx;
    cosine = ref_idx * dot(r_in.dir, hit.n) / length(r_in.dir);
  }
  else {
    outward_n = hit.n;
    ni_over_nt = 1.f / ref_idx;
    cosine = -dot(r_in.dir, hit.n) / length(r_in.dir);
  }

  attenuation = vec3(1, 1, 1);

  float reflect_prob;
  vec3 refracted;
  if (refract(refracted, r_in.dir, outward_n, ni_over_nt)) {
    reflect_prob = schlick(cosine, ref_idx);
  }
  else {
    reflect_prob = 1.f;
  }

  if (frand() < reflect_prob) {
    scattered = ray(hit.p, reflect(normalize(r_in.dir), hit.n), r_in.time);
  }
  else {
    scattered = ray(hit.p, refracted, r_in.time);
  }


  return true;
}


bool isotropic::scatter(ray& scattered,
                         vec3& attenuation,
                         const ray& r_in,
                         const intersection& hit) const
{
  scattered = ray(hit.p, random_in_unit_sphere(), r_in.time);
  attenuation = albedo->value(hit.u, hit.p);
  return true;
}
