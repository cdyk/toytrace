#include "rectangle.h"

aabb xy_rect::bounding_box(float time0, float time1) const
{
  return aabb(vec3(p0.x, p0.y, offset - 0.0001f), vec3(p1.x, p1.y, offset + 0.0001f));
}


bool xy_rect::intersect(const ray& r, float t_min, float t_max, intersection& isec) const
{
  auto t = (offset - r.ori.z) / r.dir.z;
  if (t < t_min || t_max < t) return false;

  auto q = vec2(r.ori.x, r.ori.y) + t * vec2(r.dir.x, r.dir.y);
  if (q.x < p0.x || q.y < p0.y || p1.x < q.x || p1.y < q.y) return false;

  isec.t = t;
  isec.u = vec2(1.f / (p1.x - p0.x), 1.f / (p1.y - p0.y))* (q - p0);
  isec.p = r.at(t);
  isec.mat = mat;
  isec.n = vec3(0, 0, 1);
  return true;
}

