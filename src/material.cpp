#include "material.h"
#include "intersectable.h"

namespace {

  float frand()
  {
    return (1.f / (1.f + RAND_MAX))*float(rand());
  }

  vec3 random_in_unit_sphere()
  {
    vec3 p;
    do {
      p = 2.f*vec3(frand(), frand(), frand()) - vec3(1, 1, 1);
    } while (dot(p, p) >= 1.f);
    return p;
  }

  bool refract(vec3& refracted, const vec3& v, const vec3& n, float ni_over_nt)
  {
    vec3 w = normalize(v);
    float dt = dot(w, n);
    float discriminant = 1.f - ni_over_nt * ni_over_nt * (1.f - dt * dt);
    if (0.f < discriminant) {
      refracted = ni_over_nt * (w - dt * n) - std::sqrt(discriminant) * n;
      return true;
    }
    else {
      return false;
    }
  }

  float schlick(float cosine, float ref_idx)
  {
    float r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0)*std::pow(1 - cosine, 5);
  }

}

bool lambertian::scatter(ray& scattered,
                         vec3& attenuation,
                         const ray& r_in,
                         const intersection& hit) const
{
  vec3 target = hit.p + hit.n + random_in_unit_sphere();
  scattered = ray(hit.p, target - hit.p, r_in.time);
  attenuation = albedo->value(0, 0, hit.p);
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

