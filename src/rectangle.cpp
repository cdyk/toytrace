#include "funcs.h"
#include "rectangle.h"

float xz_rect::pdf_value(const vec3& origin, const vec3& dir) const
{
  intersection hit;
  if (this->intersect(ray(origin, dir, 0), 0.001f, max_float, hit)) {
    auto area = (p1.x - p0.x)*(p1.y - p0.y);
    auto dir_sqr_len = dot(dir, dir);
    auto dir_len = std::sqrt(dir_sqr_len);

    auto distance_squared = hit.t * hit.t * dir_sqr_len;
    auto cosine = std::abs(dot(hit.n, dir) / dir_len);

    return distance_squared / (cosine * area);
  }
  return 0.f;
}

vec3 xz_rect::random(const vec3& origin) const
{
  return vec3(p0.x + (p1.x - p0.x)*frand(),
              offset,
              p0.y + (p1.y - p0.y)*frand()) - origin;
}



aabb xy_rect::bounding_box(float time0, float time1) const
{
  return aabb(vec3(p0.x, p0.y, offset - 0.0001f), vec3(p1.x, p1.y, offset + 0.0001f));
}


aabb xz_rect::bounding_box(float time0, float time1) const
{
  return aabb(vec3(p0.x, offset - 0.0001f, p0.y), vec3(p1.x, offset + 0.0001f, p1.y));
}


aabb yz_rect::bounding_box(float time0, float time1) const
{
  return aabb(vec3(offset - 0.0001f, p0.x, p0.y), vec3(offset + 0.0001f, p1.x, p1.y));
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

bool xz_rect::intersect(const ray& r, float t_min, float t_max, intersection& isec) const
{
  auto t = (offset - r.ori.y) / r.dir.y;
  if (t < t_min || t_max < t) return false;

  auto q = vec2(r.ori.x, r.ori.z) + t * vec2(r.dir.x, r.dir.z);
  if (q.x < p0.x || q.y < p0.y || p1.x < q.x || p1.y < q.y) return false;

  isec.t = t;
  isec.u = vec2(1.f / (p1.x - p0.x), 1.f / (p1.y - p0.y))* (q - p0);
  isec.p = r.at(t);
  isec.mat = mat;
  isec.n = vec3(0, 1, 0);
  return true;
}

bool yz_rect::intersect(const ray& r, float t_min, float t_max, intersection& isec) const
{
  auto t = (offset - r.ori.x) / r.dir.x;
  if (t < t_min || t_max < t) return false;

  auto q = vec2(r.ori.y, r.ori.z) + t * vec2(r.dir.y, r.dir.z);
  if (q.x < p0.x || q.y < p0.y || p1.x < q.x || p1.y < q.y) return false;

  isec.t = t;
  isec.u = vec2(1.f / (p1.x - p0.x), 1.f / (p1.y - p0.y))* (q - p0);
  isec.p = r.at(t);
  isec.mat = mat;
  isec.n = vec3(1, 0, 0);
  return true;
}

