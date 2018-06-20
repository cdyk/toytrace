#pragma once
#include "ray.h"
#include "quat.h"

class camera
{
public:

  camera() = delete;

  camera(const vec3& source, const vec3& target, const vec3& up, float vfov, float aspect, float aperture, float time0, float time1)
    : time0(time0), time1(time1)
  {
    setLens(vfov, aspect, aperture);
    lookAt(source, target, up);
  }

  void lookAt(const vec3& source, const vec3& target, const vec3& = vec3(0,1,0));

  void setLens(float vfov, float aspect, float aperture);

  ray getRay(float u, float v) const;

  vec3 origin;
  quat orientation;
  float time0;
  float time1;
  
  float lens_radius;
  float focus_dist;
  float half_width;
  float half_height;
};