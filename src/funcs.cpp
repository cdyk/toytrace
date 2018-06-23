#include <cmath>

#include "funcs.h"
#include "vec3.h"

inline float frand()
{
  return (1.f / (1.f + RAND_MAX))*float(rand());
}

vec3 random_in_unit_disc()
{
  vec3 p;
  do {
    p = 2.f*vec3(frand(), frand(), 0) - vec3(1, 1, 0);
  } while (dot(p, p) >= 1.f);
  return p;
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
