#include "stb_image_write.h"
#include <cstdint>
#include <cstdio>
#include "funcs.h"
#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include <cstdlib>

namespace {

  float frand()
  {
    return (1.f / (1.f + RAND_MAX))*float(rand());
  }


  vec3 color(const ray& r, intersectable* world)
  {
    intersection hit;

    if (world->intersect(r, 0.f, std::numeric_limits<float>::max(), hit)) {
      return 0.5f*(hit.n + vec3(1.f, 1.f, 1.f));
    }

    vec3 v = normalize(r.dir);
    auto t = 0.5f*(v.y + 1.f);
    return mix(vec3(1.f, 1.f, 1.f), vec3(0.5f, 0.7f, 1.f), t);
  }

}

int main(int argc, char** argv)
{
  const char* filename = "output.png";
  const unsigned w = 200;
  const unsigned h = 100;
  const unsigned s = 100;

  uint8_t image[3 * w * h];

  auto * world = new intersectable_container();
  world->items.push_back(new sphere(vec3(0, 0, -1), 0.5f));
  world->items.push_back(new sphere(vec3(0, -100.5, -2), 100.f));

  vec3 llcorner(-2.f, -1.f, -1.f);
  vec3 horizontal(4.f, 0.f, 0.f);
  vec3 vertical(0.f, 2.f, 0.f);
  vec3 origin(0.f, 0.f, 0.f);

  for (unsigned j = 0; j < h; j++) {
    for (unsigned i = 0; i < w; i++) {

      vec3 col;
      for (unsigned k = 0; k < s; k++) {
        auto u = float(i + frand()) / float(w);
        auto v = float(j + frand()) / float(h);

        ray r(origin, llcorner + u * horizontal + v * vertical);
        col = col + color(r, world);
      }
      col = (1.f / s)*col;

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