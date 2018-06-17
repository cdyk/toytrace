#include "intersectable.h"

bool intersectable_container::intersect(const ray& r, float t_min, float t_max, intersection& hit) const
{
  intersection temp_hit;

  bool hit_anything = false;
  bool closest_so_far = t_max;
  for (auto & item : items) {
    if (item->intersect(r, t_min, closest_so_far, temp_hit)) {
      hit_anything = true;
      closest_so_far = temp_hit.t;
      hit = temp_hit;
    }
  }
  return hit_anything;
}
