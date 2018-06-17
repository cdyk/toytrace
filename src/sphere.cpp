#include "sphere.h"

bool sphere::intersect(const ray& r, float t_min, float t_max, intersection& rec) const
{
  vec3 oc = r.ori - center;
  float a = dot(r.dir, r.dir);
  float b = 2.f * dot(oc, r.dir);
  float c = dot(oc, oc) - radius * radius;
  float disc = b * b - 4 * a*c;
  if (0.f < disc) {
    auto t = (-b - std::sqrt(disc)) / (2.f*a);
    if (t_min < t && t < t_max) {
      rec.t = t;
      rec.p = r.at(t);
      rec.n = (1.f / radius)*(rec.p - center);
      return true;
    }
    t = (-b + std::sqrt(disc)) / (2.f*a);
    if (t_min < t && t < t_max) {
      rec.t = t;
      rec.p = r.at(t);
      rec.n = (1.f / radius)*(rec.p - center);
      return true;
    }
  }
  return false;
}
