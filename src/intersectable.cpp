#include <cmath>

#include "intersectable.h"
#include "funcs.h"
#include "material.h"

bool intersectable_container::intersect(const ray& r, float t_min, float t_max, intersection& hit) const
{
  intersection temp_hit;

  bool hit_anything = false;
  float closest_so_far = t_max;
  for (auto & item : items) {
    if (item->intersect(r, t_min, closest_so_far, temp_hit)) {
      hit_anything = true;
      closest_so_far = temp_hit.t;
      hit = temp_hit;
    }
  }
  return hit_anything;
}

aabb intersectable_container::bounding_box(float time0, float time1) const
{
  aabb rv;
  for (auto & item : items) {
    rv.include(item->bounding_box(time0, time1));
  }
  return rv;
}

bool translate::intersect(const ray& r, float t_min, float t_max, intersection& isec) const
{
  ray shifted(r.ori - shift, r.dir, r.time);
  if (obj->intersect(shifted, t_min, t_max, isec)) {
    isec.p = isec.p + shift;
    return true;
  }
  return false;
}

aabb translate::bounding_box(float time0, float time1) const
{
  auto bbox = obj->bounding_box(time0, time1);
  if (!bbox.empty()) {
    return aabb(bbox.min + shift, bbox.max + shift);
  }
  return bbox;
}

rotate_y::rotate_y(intersectable* obj, float theta) :
  obj(obj),
  theta_cos(std::cos(radians(theta))),
  theta_sin(std::sin(radians(theta)))
{
}

bool rotate_y::intersect(const ray& r, float t_min, float t_max, intersection& isec) const
{
  vec3 ori(theta_cos * r.ori.x - theta_sin * r.ori.z,
           r.ori.y,
           theta_sin * r.ori.x + theta_cos * r.ori.z);
  vec3 dir(theta_cos * r.dir.x - theta_sin * r.dir.z,
           r.dir.y,
           theta_sin * r.dir.x + theta_cos * r.dir.z);
  if (obj->intersect(ray(ori, dir, r.time), t_min, t_max, isec)) {

    isec.p = vec3(theta_cos * isec.p.x + theta_sin * isec.p.z,
                  isec.p.y,
                  -theta_sin * isec.p.x + theta_cos * isec.p.z);

    isec.n = vec3(theta_cos * isec.n.x + theta_sin * isec.n.z,
                  isec.n.y,
                  -theta_sin * isec.n.x + theta_cos * isec.n.z);

    return true;
  }
  return false;
}

aabb rotate_y::bounding_box(float time0, float time1) const
{
  auto bbox = obj->bounding_box(time0, time1);
  if (bbox.empty()) return bbox;

  aabb newbox;
  for(int k=0; k<2; k++) {
    for (int j =0; j < 2; j++) {
      for (int i = 0; i < 2; i++) {
        vec3 p(i == 0 ? bbox.min.x : bbox.max.x,
               j == 0 ? bbox.min.y : bbox.max.y,
               k == 0 ? bbox.min.z : bbox.max.z);
        vec3 q(theta_cos * p.x + theta_sin * p.z,
               p.y,
               -theta_sin * p.x + theta_cos * p.z);
        newbox.include(q);
      }
    }
  }
  return newbox;
}

constant_medium::constant_medium(intersectable* boundary, float density, texture* phase_func) :
  boundary(boundary),
  density(density),
  phase_func(new isotropic(phase_func))
{ }


bool constant_medium::intersect(const ray& r, float t_min, float t_max, intersection& isec) const
{

  intersection enter_hit;
  if (boundary->intersect(r, -FLT_MAX, FLT_MAX, enter_hit)) {  // find any hit

    intersection exit_hit;
    if (boundary->intersect(r, enter_hit.t + 0.0001f, FLT_MAX, exit_hit)) {  // we're inside

      enter_hit.t = mmax(enter_hit.t, t_min);
      exit_hit.t = mmin(exit_hit.t, t_max);

      if (exit_hit.t < enter_hit.t) return false;

      auto distance_inside_boundary = (exit_hit.t - enter_hit.t)*length(r.dir);

      auto probabilistic_hit_distance = -(1.f / density)*log(frand());

      if (probabilistic_hit_distance < distance_inside_boundary) {

        isec.t = enter_hit.t + probabilistic_hit_distance / length(r.dir);
        isec.u = vec2(0);
        isec.p = r.at(isec.t);
        isec.n = vec3(1, 0, 0);
        isec.mat = phase_func;

        return true;
      }
    }
  }
  return false;
}
