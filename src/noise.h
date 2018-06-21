#pragma once
#include "vec3.h"

class perlin
{
public:

  static float noise(const vec3& p);

  static float turbulence(const vec3& p, unsigned octaves = 7);

private:

  static struct shared_t
  {
    shared_t();
    vec3* ranfloat;
    int* perm_x;
    int* perm_y;
    int* perm_z;
  } shared;
};