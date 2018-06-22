#include "sphere.h"
#include <cstdio>
#include <cassert>

namespace {

  constexpr float pi = float(3.14159265358979323846264338327950288);
  constexpr float pi_over_two = float(3.14159265358979323846264338327950288 / 2.0);
  constexpr float one_over_pi = float(1.0 / (3.14159265358979323846264338327950288));
  constexpr float one_over_two_pi = float(1.0 / (2 * 3.14159265358979323846264338327950288));


}

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

      float phi = atan2(rec.n.z, rec.n.x);
      float theta = asin(rec.n.y);
      rec.u.x = 1.f - one_over_two_pi * (phi + pi);
      rec.u.y = 1.f - one_over_pi * (theta + pi_over_two);

      return true;
    }
    t = (-b + std::sqrt(disc)) / a;
    if (t_min < t && t < t_max) {
      rec.t = t;
      rec.p = r.at(t);
      rec.n = (1.f / radius)*(rec.p - center);
      rec.mat = mat;

      float phi = atan2(rec.n.z, rec.n.x);
      float theta = asin(rec.n.y);
      rec.u.x = 1.f - one_over_two_pi * (phi + pi);
      rec.u.y = 1.f - one_over_pi * (theta + pi_over_two);

      return true;
    }
  }
  return false;
}

aabb sphere::bounding_box(float time0, float time1) const
{
  return aabb(center - vec3(radius),
              center + vec3(radius));
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

      float phi = atan2(rec.n.z, rec.n.x);
      float theta = asin(rec.n.y);
      rec.u.x = 1.f - one_over_two_pi * (phi + pi);
      rec.u.y = 1.f - one_over_pi * (theta + pi_over_two);

      return true;
    }
    t = (-b + std::sqrt(disc)) / a;
    if (t_min < t && t < t_max) {
      rec.t = t;
      rec.p = r.at(t);
      rec.n = (1.f / radius)*(rec.p - ctr);
      rec.mat = mat;

      float phi = atan2(rec.n.z, rec.n.x);
      float theta = asin(rec.n.y);
      rec.u.x = 1.f - one_over_two_pi * (phi + pi);
      rec.u.y = 1.f - one_over_pi * (theta + pi_over_two);

      return true;
    }
  }
  return false;
}

aabb moving_sphere::bounding_box(float tim0, float tim1) const
{
  auto t0 = (tim0 - time0) / (time1 - time0);
  auto c0 = (1.f - t0)*center0 + t0 * center1;

  auto t1 = (tim1 - time0) / (time1 - time0);
  auto c1 = (1.f - t1)*center0 + t1 * center1;

  vec3 min(c0.x < c1.x ? c0.x : c1.x,
           c0.y < c1.y ? c0.y : c1.y,
           c0.z < c1.z ? c0.z : c1.z);

  vec3 max(c0.x < c1.x ? c1.x : c0.x,
           c0.y < c1.y ? c1.y : c0.y,
           c0.z < c1.z ? c1.z : c0.z);

  return aabb(min - vec3(radius),
              max + vec3(radius));
}
