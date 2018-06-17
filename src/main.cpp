#include "stb_image_write.h"
#include <cstdint>
#include <cstdio>
#include "funcs.h"
#include "vec3.h"
#include "ray.h"

namespace {


  vec3 color(const ray& r) {
    vec3 v = normalize(r.direction());
    float t = 0.5f*(v.y + 1.f);
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