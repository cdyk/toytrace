#include <algorithm>
#include <cassert>
#include "bvh.h"

namespace
{

  struct sort_item
  {
    float key;
    unsigned index;
  };


}


bvh::bvh(const std::vector<intersectable*>& items, float t0, float t1, unsigned axis)
{
  unsigned N = unsigned(items.size());
  if (N == 1) {
    children[0] = items[0];
    return;
  }

  std::vector<sort_item> sort_items(N);

  for (unsigned i = 0; i < N; i++) {
    auto item_bbox = items[i]->bounding_box(t0, t1);
    bbox.include(item_bbox);
    assert(!item_bbox.empty());
    sort_items[i].key = item_bbox.min[axis] + item_bbox.max[axis];
    sort_items[i].index = i;
  }
  std::sort(sort_items.begin(), sort_items.end(), [](const sort_item & a, const sort_item & b) { return a.key < b.key; });

  if (N == 2) {
    children[0] = items[sort_items[0].index];
    children[1] = items[sort_items[1].index];
    return;
  }

  auto M = N / 2;
  std::vector<intersectable*> tmp;
  tmp.reserve(M + 1);

  tmp.resize(M);
  for (unsigned i = 0; i < M; i++) {
    tmp[i] = items[sort_items[i].index];
  }
  children[0] = new bvh(tmp, t0, t1, (axis + 1)%3);

  tmp.resize(N-M);
  for (unsigned i = M; i < N; i++) {
    tmp[i-M] = items[sort_items[i].index];
  }
  children[1] = new bvh(tmp, t0, t1, (axis + 1)%3);
}

bool bvh::intersect(const ray& r, float t_min, float t_max, intersection& isec) const
{
  if (bbox.intersects(r, t_min, t_max) == false) return false;

  intersection isec_l, isec_r;
  bool hit_l = children[0]->intersect(r, t_min, t_max, isec_l);
  bool hit_r = children[1] && children[1]->intersect(r, t_min, t_max, isec_r);
  if (hit_l && hit_r) {
    isec = isec_l.t < isec_r.t ? isec_l : isec_r;
    return true;
  }
  else if (hit_l) {
    isec = isec_l;
    return true;
  }
  else if (hit_r) {
    isec = isec_r;
    return true;
  }
  return false;
}

aabb bvh::bounding_box(float time0, float time1) const
{
  return bbox;
}

