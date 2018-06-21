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
  ranfloat = new vec3[256];
  for (unsigned i = 0; i < 256; i++) {
    ranfloat[i] = normalize(vec3((2.f / (1.f + RAND_MAX))*float(rand()) - 1.f,
                                 (2.f / (1.f + RAND_MAX))*float(rand()) - 1.f,
                                 (2.f / (1.f + RAND_MAX))*float(rand()) - 1.f));
  }
  perm_x = create_permutation_table();
  perm_y = create_permutation_table();
  perm_z = create_permutation_table();
}

float perlin::noise(const vec3& p)
{
  int i = int(floor(p.x));
  int j = int(floor(p.y));
  int k = int(floor(p.z));
  float u = p.x - i;
  float v = p.y - j;
  float w = p.z - k;
  float uu = u * u*(3.f - 2.f*u);
  float vv = v * v*(3.f - 2.f*v);
  float ww = w * w*(3.f - 2.f*w);

  vec3 c[2][2][2];
  for (int kk = 0; kk < 2; kk++) {
    for (int jj = 0; jj < 2; jj++) {
      for (int ii = 0; ii < 2; ii++) {
        c[kk][jj][ii] = shared.ranfloat[shared.perm_x[(i + ii) & 255] ^ shared.perm_y[(j + jj) & 255] ^ shared.perm_z[(k + kk) & 255]];
      }
    }
  }

  float acc = 0.f;
  for (int kk = 0; kk < 2; kk++) {
    for (int jj = 0; jj < 2; jj++) {
      for (int ii = 0; ii < 2; ii++) {
        vec3 dir(u - ii, v - jj, w - kk);
        float t = dot(dir, c[kk][jj][ii]);
        acc += (ii*uu + (1 - ii)*(1.f - uu)) * (jj*vv + (1 - jj)*(1.f - vv)) * (kk*ww + (1 - kk)*(1.f - ww)) * t;
      }
    }
  }

  return acc;
}

float perlin::turbulence(const vec3& p, unsigned octaves)
{
  float acc = 0;
  auto t = p;
  float w = 1.f;
  for (unsigned i = 0; i < octaves; i++) {
    acc += w * noise(t);
    w *= 0.5f;
    t = 2.f*t;
  }
  return std::abs(acc);
}