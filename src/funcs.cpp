#include <cmath>
#include <atomic>
#include <cassert>

#include "funcs.h"
#include "vec3.h"

float frand()
{
  static std::atomic<unsigned> counter = 0;
  thread_local uint64_t state;
  thread_local bool initialized = false;
  if (!initialized) {
    state = counter.fetch_add(1);
    initialized = true;
  }
  state = (25214903917ull*state + 11ull) & ((1ull << 48) - 1ull);
  auto f = float((1.0 / (1ull << 48))*double(state));
  assert(f <= 1.f);
  return f;
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

// p(dir) = cos(theta)/Pi.
vec3 random_cosine_direction()
{
  float r1 = frand();
  float r2 = frand();
  float z = std::sqrt(1.f - r2);
  float phi = two_pi * r1;
  float two_sqrt_r2 = std::sqrt(r2);
  float x = two_sqrt_r2 * std::cos(phi);
  float y = two_sqrt_r2 * std::sin(phi);
  return vec3(x, y, z);
}


void orthonormal(vec3& u, vec3& v, vec3& w, const vec3& d)
{
  w = normalize(d);
  auto a = 0.9f < std::abs(w.x) ? vec3(0, 1, 0) : vec3(1, 0, 0);
  v = normalize(cross(w, a));
  u = cross(w, v);
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
