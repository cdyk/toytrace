#include <cmath>
#include <limits>
#include "funcs.h"
#include "camera.h"

void camera::setLens(float vfov, float aspect, float aperture)
{
  this->aperture = aperture;
  half_height = std::tan(0.5f*radians(vfov));
  half_width = aspect * half_height;
}

void camera::lookAt(const vec3& source, const vec3& target, const vec3& up)
{
  origin = source;
  auto direction = target-source;
  focus_dist = length(direction);
  orientation = greatCircle(vec3(0, 0, -1), normalize(direction));

  // transform up back to local camera frame.
  vec3 a = transform(conjugate(orientation), up);

  // and try to counter by pre-rotating the camera around z.
  vec3 b(a.x, a.y, 0);
  auto l = length(b);
  if (std::numeric_limits<float>::epsilon() < l) {
    auto q = axisAngle(vec3(0, 0, 1), std::acos(a.y / l));
    orientation = orientation * q;
  }
  
}

ray camera::getRay(float u, float v)
{
  vec3 dir(focus_dist*half_width*(2.f*u - 1.f),
           focus_dist*half_height*(2.f*v - 1.f),
           -focus_dist);

  return ray(origin, transform(orientation, dir));

  //return ray(origin, transform(orientation, llcorner + u * horizontal + v * vertical));
}
