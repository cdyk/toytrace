#pragma once
#include "ray.h"
#include "quat.h"

class camera
{
public:

  camera()
  {
    setLens(120.f, 1.f, 2.f);
  }

  void lookAt(const vec3& source, const vec3& target, const vec3& = vec3(0,1,0));

  void setLens(float vfov, float aspect, float aperture);

  ray getRay(float u, float v);

  vec3 origin;
  quat orientation;

  float aperture = 2.f;
  float focus_dist = 1.f;

  float half_width;
  float half_height;
};