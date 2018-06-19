#include "stb_image_write.h"
#include <cstdint>
#include <cstdio>
#include "funcs.h"
#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include <cstdlib>

namespace {

  float frand()
  {
    return (1.f / (1.f + RAND_MAX))*float(rand());
  }

  vec3 random_in_unit_sphere()
  {
    vec3 p;
    do {
      p = 2.f*vec3(frand(), frand(), frand()) - vec3(1, 1, 1);
    } while (dot(p, p) >= 1.f);
    return p;
  }

  vec3 color(const ray& r, intersectable* world, unsigned depth)
  {
    intersection hit;
    if (world->intersect(r, 0.001f, std::numeric_limits<float>::max(), hit)) {
      ray scattered;
      vec3 attenuation;

      if (depth && hit.mat->scatter(scattered, attenuation, r, hit)) {
        return attenuation * color(scattered, world, depth - 1);
      }
      else {
        return vec3(0, 0, 0);
      }
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
  const unsigned s = 5;

  uint8_t image[3 * w * h];

  auto * world = new intersectable_container();
  world->items.push_back(new sphere(vec3(0, 0, -1), 0.5f, new lambertian(vec3(0.8f, 0.3f, 0.3f))));
  world->items.push_back(new sphere(vec3(0, -100.5, -1), 100.f, new lambertian(vec3(0.8f, 0.8f, 0.0f))));
  world->items.push_back(new sphere(vec3(1, 0, -1), 0.5f, new metal(vec3(0.8f, 0.6f, 0.2f), 0.6f)));
  world->items.push_back(new sphere(vec3(-1, 0, -1), 0.5f, new dielectric(1.5f)));
  world->items.push_back(new sphere(vec3(-1, 0, -1), -0.45f, new dielectric(1.5f)));

  camera cam;
  cam.setLens(20.f, float(w) / float(h), 2.f);
  cam.lookAt(vec3(3, 3, 2), vec3(0, 0, -1));

  for (unsigned j = 0; j < h; j++) {
    for (unsigned i = 0; i < w; i++) {

      vec3 col;
      for (unsigned k = 0; k < s; k++) {
        auto u = float(i + frand()) / float(w);
        auto v = float(j + frand()) / float(h);

        auto r = cam.getRay(u, v);

        col = col + color(r, world, 20);
      }
      col = (1.f / s)*col;

      image[3 * (j*w + i) + 0] = uint8_t(255.f*saturate(std::sqrt(col.r)));
      image[3 * (j*w + i) + 1] = uint8_t(255.f*saturate(std::sqrt(col.g)));
      image[3 * (j*w + i) + 2] = uint8_t(255.f*saturate(std::sqrt(col.b)));
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