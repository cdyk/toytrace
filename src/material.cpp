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


}

bool lambertian::scatter(ray& scattered,
                         vec3& attenuation,
                         const ray& r_in,
                         const intersection& hit) const
{
  vec3 target = hit.p + hit.n + random_in_unit_sphere();
  scattered = ray(hit.p, target - hit.p);
  attenuation = albedo;
  return true;
}


bool metal::scatter(ray& scattered,
                    vec3& attenuation,
                    const ray& r_in,
                    const intersection& hit) const
{
  scattered = ray(hit.p, reflect(normalize(r_in.dir), hit.n) + fuzz*random_in_unit_sphere());
  attenuation = albedo;
  return 0 < dot(scattered.dir, hit.n);
}
