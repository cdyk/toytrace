#include <cstdlib>
#include <cassert>

#include "noise.h"

namespace {

  int* create_permutation_table()
  {
    auto * p = new int[256];
    for (unsigned i = 0; i < 256; i++) p[i] = i;

    for (unsigned i = 0; i < 255; i++) {
      unsigned j = i + std::rand() / (RAND_MAX / (256 - i) + 1);
      assert(j < 256);
      auto t = p[i];
      p[i] = p[j];
      p[j] = t;
    }
    return p;
  }
}

perlin::shared_t perlin::shared;

perlin::shared_t::shared_t()
{
  ranfloat = new float[256];
  for (unsigned i = 0; i < 256; i++) {
    ranfloat[i] = (1.f / (1.f + RAND_MAX))*float(rand());
  }
  perm_x = create_permutation_table();
  perm_y = create_permutation_table();
  perm_z = create_permutation_table();
}

float perlin::noise(const vec3& p)
{
  int i = floor(p.x);
  int j = floor(p.y);
  int k = floor(p.z);
  float u = p.x - i;
  float v = p.y - j;
  float w = p.z - k;
  u = u * u*(3.f - 2.f*u);
  v = v * v*(3.f - 2.f*v);
  w = w * w*(3.f - 2.f*w);

  auto t000 = shared.ranfloat[shared.perm_x[(i + 0) & 255] ^ shared.perm_y[(j + 0) & 255] ^ shared.perm_z[(k + 0) & 255]];
  auto t001 = shared.ranfloat[shared.perm_x[(i + 1) & 255] ^ shared.perm_y[(j + 0) & 255] ^ shared.perm_z[(k + 0) & 255]];
  auto t00 = (1.f - u)*t000 + u * t001;

  auto t010 = shared.ranfloat[shared.perm_x[(i + 0) & 255] ^ shared.perm_y[(j + 1) & 255] ^ shared.perm_z[(k + 0) & 255]];
  auto t011 = shared.ranfloat[shared.perm_x[(i + 1) & 255] ^ shared.perm_y[(j + 1) & 255] ^ shared.perm_z[(k + 0) & 255]];
  auto t01 = (1.f - u)*t010 + u * t011;
  auto t0 = (1.f - v)*t00 + v * t01;

  auto t100 = shared.ranfloat[shared.perm_x[(i + 0) & 255] ^ shared.perm_y[(j + 0) & 255] ^ shared.perm_z[(k + 1) & 255]];
  auto t101 = shared.ranfloat[shared.perm_x[(i + 1) & 255] ^ shared.perm_y[(j + 0) & 255] ^ shared.perm_z[(k + 1) & 255]];
  auto t10 = (1.f - u)*t100 + u * t101;

  auto t110 = shared.ranfloat[shared.perm_x[(i + 0) & 255] ^ shared.perm_y[(j + 1) & 255] ^ shared.perm_z[(k + 1) & 255]];
  auto t111 = shared.ranfloat[shared.perm_x[(i + 1) & 255] ^ shared.perm_y[(j + 1) & 255] ^ shared.perm_z[(k + 1) & 255]];
  auto t11 = (1.f - u)*t110 + u * t111;
  auto t1 = (1.f - v)*t10 + v * t11;

  auto t = (1.f - w)*t0 + w * t1;

  return t;
}