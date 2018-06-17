#pragma once
#include "vec3.h"

struct mat3
{
  mat3()
  {
    data[0] = 1.f; data[1] = 0.f; data[2] = 0.f;
    data[3] = 0.f; data[4] = 1.f; data[5] = 0.f;
    data[6] = 0.f; data[7] = 0.f; data[8] = 1.f;
  }

  mat3(float c0r0, float c0r1, float c0r2,
       float c1r0, float c1r1, float c1r2,
       float c2r0, float c2r1, float c2r2)
  {
    data[0] = c0r0; data[1] = c0r1; data[2] = c0r2;
    data[3] = c1r0; data[4] = c1r1; data[5] = c1r2;
    data[6] = c2r0; data[7] = c2r1; data[8] = c2r2;
  }

  union {
    float data[3*3];
    vec3 col[3];
  };
};
