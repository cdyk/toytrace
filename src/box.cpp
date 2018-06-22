#include "box.h"
#include "rectangle.h"

box::box(const vec3& p0, const vec3& p1, material* mat) :
  p0(p0),
  p1(p1),
  mat(mat)
{
  walls.items.push_back(new xy_rect(vec2(p0.x, p0.y), vec2(p1.x, p1.y), p1.z, mat));
  walls.items.push_back(new normal_flip(new xy_rect(vec2(p0.x, p0.y), vec2(p1.x, p1.y), p0.z, mat)));

  walls.items.push_back(new xz_rect(vec2(p0.x, p0.z), vec2(p1.x, p1.z), p1.y, mat));
  walls.items.push_back(new normal_flip(new xz_rect(vec2(p0.x, p0.z), vec2(p1.x, p1.z), p0.y, mat)));

  walls.items.push_back(new yz_rect(vec2(p0.y, p0.z), vec2(p1.y, p1.z), p1.x, mat));
  walls.items.push_back(new normal_flip(new yz_rect(vec2(p0.y, p0.z), vec2(p1.y, p1.z), p0.x, mat)));
}
