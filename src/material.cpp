#include "funcs.h"
#include "pdf.h"
#include "material.h"
#include "intersectable.h"


bool lambertian::scatter(ScatterData& scatterData,
                         const ray& r_in,
                         const intersection& hit) const
{
  vec3 u, v, w;
  orthonormal(u, v, w, hit.n);

  auto dir_l = random_cosine_direction();

  vec3 on_hemisphere = dir_l.x*u + dir_l.y*v + dir_l.z*w;

  scatterData.isSpecular = false;
  scatterData.attenuation = albedo->value(hit.u, hit.p);
  scatterData.pdf = new cosine_pdf(hit.n);

  return true;
}

float lambertian::scattering_pdf(const ray& r_in, const intersection& hit, const ray& r_scattered) const
{
  auto cosine = dot(hit.n, normalize(r_scattered.dir)); // fixme: make dir always normalized
  return one_over_pi * mmax(0.f, cosine);
}


bool metal::scatter(ScatterData& scatterData,
                    const ray& r_in,
                    const intersection& hit) const
{
  auto reflected = reflect(normalize(r_in.dir), hit.n);

  scatterData.specularRay = ray(hit.p, reflected + fuzz * random_in_unit_sphere(), r_in.time);
  scatterData.attenuation = albedo;
  scatterData.isSpecular = true;
  scatterData.pdf = nullptr;

  return true;
}

#if 0

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
#endif

vec3 diffuse_light::emitted(const ray& r_in, const intersection& hitData, const vec2& t, const vec3& p) const
{
  if (dot(r_in.dir, hitData.n) < 0.f) {
    return emit->value(t, p);
  }
  else {
    return 0.f;
  }
}
