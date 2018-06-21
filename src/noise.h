#pragma once
#include "vec3.h"

class perlin
{
public:

  static float noise(const vec3& p);

private:

  static struct shared_t
  {
    shared_t();
    float* ranfloat;
    int* perm_x;
    int* perm_y;
    int* perm_z;
  } shared;
};