#include "stb_image_write.h"
#include <cstdint>
#include <cstdio>
#include "funcs.h"
#include "vec3.h"
#include "ray.h"

namespace {

  float intersectSphere(const vec3& center, const float radius, const ray& r)
  {
    vec3 oc = r.ori - center;
    float a = dot(r.dir, r.dir);
    float b = 2.f * dot(oc, r.dir);
    float c = dot(oc, oc) - radius * radius;
    float disc = b * b - 4 * a*c;
    if (disc < 0) {
      return -1.f;
    }
    else {
      return (-b - std::sqrt(disc)) / (2.f*a);
    }
  }


  vec3 color(const ray& r) {
    auto sphere_origin = vec3(0.f, 0.f, -1.f);

    auto t = intersectSphere(sphere_origin, 0.5f, r);
    if (0.f < t) {
      vec3 n = normalize(r.at(t) - sphere_origin);
      return 0.5f*(n + vec3(1.f, 1.f, 1.f));
    }
    vec3 v = normalize(r.dir);
    t = 0.5f*(v.y + 1.f);
    return mix(vec3(1.f, 1.f, 1.f), vec3(0.5f, 0.7f, 1.f), t);
  }

}

int main(int argc, char** argv)
{
  const char* filename = "output.png";
  const unsigned w = 200;
  const unsigned h = 100;

  uint8_t image[3 * w * h];



  vec3 llcorner(-2.f, -1.f, -1.f);
  vec3 horizontal(4.f, 0.f, 0.f);
  vec3 vertical(0.f, 2.f, 0.f);
  vec3 origin(0.f, 0.f, 0.f);

  for (unsigned j = 0; j < h; j++) {
    for (unsigned i = 0; i < w; i++) {
      auto u = float(i) / float(w);
      auto v = float(j) / float(h);

      ray r(origin, llcorner + u * horizontal + v * vertical);
      auto col = color(r);

      image[3 * (j*w + i) + 0] = uint8_t(255.f*saturate(col.r));
      image[3 * (j*w + i) + 1] = uint8_t(255.f*saturate(col.g));
      image[3 * (j*w + i) + 2] = uint8_t(255.f*saturate(col.b));
    }
  }


  vec3 a, b;

  auto t = a - b;

  stbi_flip_vertically_on_write(1);
  if (stbi_write_png(filename, w, h, 3, image, 3*w)) {
    fprintf(stderr, "Failed to write png file '%s'", filename);
    return -1;
  }

  return 0;
}