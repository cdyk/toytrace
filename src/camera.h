#pragma once
#include "ray.h"

class camera
{
public:

  camera() :
    origin(0.f, 0.f, 0.f),
    llcorner(-2.f, -1.f, -1.f),
    horizontal(4.f, 0.f, 0.f),
    vertical(0.f, 2.f, 0.f)
  {
  }

  ray getRay(float u, float v)
  {
    return ray(origin, llcorner + u * horizontal + v * vertical);
  }

  vec3 origin;
  vec3 llcorner;
  vec3 horizontal;
  vec3 vertical;

};