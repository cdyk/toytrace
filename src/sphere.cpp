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
      rec.mat = mat;
      return true;
    }
    t = (-b + std::sqrt(disc)) / a;
    if (t_min < t && t < t_max) {
      rec.t = t;
      rec.p = r.at(t);
      rec.n = (1.f / radius)*(rec.p - center);
      rec.mat = mat;
      return true;
    }
  }
  return false;
}

bool moving_sphere::intersect(const ray& r, float t_min, float t_max, intersection& rec) const
{
  // a = o-p
  // <a + td, a + td> = r^2
  // t^2<d,d> + 2t<a,d> + <a,a> - r^2

  auto t = (r.time - time0) / (time1 - time0);
  auto ctr = (1.f - t)*center0 + t * center1;

  vec3 oc = r.ori - ctr;
  float a = dot(r.dir, r.dir);
  float b = dot(oc, r.dir);
  float c = dot(oc, oc) - radius * radius;
  float disc = b * b - a * c;
  if (0.f < disc) {
    auto t = (-b - std::sqrt(disc)) / a;
    if (t_min < t && t < t_max) {
      rec.t = t;
      rec.p = r.at(t);
      rec.n = (1.f / radius)*(rec.p - ctr);
      rec.mat = mat;
      return true;
    }
    t = (-b + std::sqrt(disc)) / a;
    if (t_min < t && t < t_max) {
      rec.t = t;
      rec.p = r.at(t);
      rec.n = (1.f / radius)*(rec.p - ctr);
      rec.mat = mat;
      return true;
    }
  }
  return false;
}
