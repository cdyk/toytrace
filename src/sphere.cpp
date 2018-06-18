#include "sphere.h"
#include <cstdio>
#include <cassert>

bool sphere::intersect(const ray& r, float t_min, float t_max, intersection& rec) const
{
  // a = o-p
  // <a + td, a + td> = r^2
  // t^2<d,d> + 2t<a,d> + <a,a> - r^2

  vec3 oc = r.ori - center;
  float a = dot(r.dir, r.dir);
  float b = dot(oc, r.dir);
  float c = dot(oc, oc) - radius * radius;
  float disc = b * b - a*c;
  if (0.f < disc) {
    auto t = (-b - std::sqrt(disc)) / a;
    if (t_min < t && t < t_max) {
      rec.t = t;
      rec.p = r.at(t);
      rec.n = (1.f / radius)*(rec.p - center);
      return true;
    }
    t = (-b + std::sqrt(disc)) / a;
    if (t_min < t && t < t_max) {
      rec.t = t;
      rec.p = r.at(t);
      rec.n = (1.f / radius)*(rec.p - center);
      return true;
    }
  }
  return false;
}
