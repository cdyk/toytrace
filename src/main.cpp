#include "stb_image.h"
#include "stb_image_write.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <thread>
#include <chrono>

#include "funcs.h"
#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "rectangle.h"
#include "camera.h"
#include "material.h"
#include "bvh.h"
#include "box.h"

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

  vec3 color(const ray& r, const intersectable* world, unsigned depth)
  {
    intersection hit;
    if (world->intersect(r, 0.001f, std::numeric_limits<float>::max(), hit)) {
      ray scattered;
      vec3 attenuation;

      auto emitted = hit.mat->emitted(hit.t, hit.p);
      if (depth && hit.mat->scatter(scattered, attenuation, r, hit)) {
        return emitted + attenuation * color(scattered, world, depth - 1);
      }
      else {
        return emitted;
      }
    }

    vec3 v = normalize(r.dir);
    auto t = 0.5f*(v.y + 1.f);
    //return mix(vec3(1.f, 1.f, 1.f), vec3(0.5f, 0.7f, 1.f), t);
    return vec3(0, 0, 0);
  }

  struct setup
  {
    camera* camera;
    intersectable* world;
  };

  setup* create_world_random(float aspect)
  {
    auto * world =  new intersectable_container();

    world->items.push_back(new sphere(vec3(0, -1000, 0), 1000, new lambertian(new checker_texture(new constant_texture(vec3(0.2f, 0.3f, 0.1f)),
                                                                                                  new constant_texture(vec3(0.9f, 0.9f, 0.9f))))));

    for (int a = -11; a < 11; a++) {
      for (int b = -11; b < 11; b++) {
        float choose_mat = frand();
        vec3 center(a + 0.9f*frand(), 0.2f, b + 0.9f*frand());
        if (0.9f < distance(center, vec3(4, 0.2f, 0))) {

          if (choose_mat < 0.8) {
            vec3 albedo(frand()*frand(), frand()*frand(), frand()*frand());
            world->items.push_back(new moving_sphere(center, center + vec3(0, 0.5f*frand(), 0), 0, 1, 0.2f, new lambertian(new constant_texture(albedo))));
          }
          else if (choose_mat < 0.95f) {
            vec3 albedo(0.5f + 0.5f*frand(), 0.5f + 0.5f*frand(), 0.5f + 0.5f*frand());
            world->items.push_back(new sphere(center, 0.2f, new metal(albedo, 0.5f*frand())));
          }
          else {
            world->items.push_back(new sphere(center, 0.2f, new dielectric(1.5f)));
          }
        }
      }
    }

    world->items.push_back(new sphere(vec3(0, 1, 0), 1, new dielectric(1.5f)));
    world->items.push_back(new sphere(vec3(-4, 1, 0), 1, new lambertian(new constant_texture(vec3(0.4f, 0.2f, 0.1f)))));
    world->items.push_back(new sphere(vec3(4, 1, 0), 1, new metal(vec3(0.7f, 0.6f, 0.5f), 0.0f)));

    auto * set_up = new setup;
    set_up->camera = new camera(vec3(13, 2, 3), vec3(0, 0, 0), vec3(0, 1, 0), 20.f, aspect, 0.1f, 0, 1);
    set_up->world = new bvh(world->items, 0, 1);

    return set_up;
  }

  setup* create_world_simple(float aspect)
  {
    auto * world = new intersectable_container();

    world->items.push_back(new sphere(vec3(0, 0, -1), 0.5f, new lambertian(new constant_texture(vec3(0.8f, 0.3f, 0.3f)))));
    world->items.push_back(new sphere(vec3(0, -100.5, -1), 100.f, new lambertian(new constant_texture(vec3(0.8f, 0.8f, 0.0f)))));
    world->items.push_back(new sphere(vec3(1, 0, -1), 0.5f, new metal(vec3(0.8f, 0.6f, 0.2f), 0.6f)));
    world->items.push_back(new sphere(vec3(-1, 0, -1), 0.5f, new dielectric(1.5f)));
    world->items.push_back(new sphere(vec3(-1, 0, -1), -0.45f, new dielectric(1.5f)));

    auto * set_up = new setup;
    set_up->camera = new camera(vec3(0, 0, 0), vec3(0, 0, -1), vec3(0, 1, 0), 120.f, aspect, 0.1f, 0, 1);
    set_up->world = world;

    return set_up;
  }

  setup* two_spheres(float aspect)
  {
    texture * checker = new checker_texture(new constant_texture(vec3(0.2f, 0.3f, 0.1f)),
                                            new constant_texture(vec3(0.9f, 0.9f, 0.9f)));

    auto * world = new intersectable_container();
    world->items.push_back(new sphere(vec3(0,-10, 0), 10, new lambertian(checker)));
    world->items.push_back(new sphere(vec3(0, 10, 0), 10, new lambertian(checker)));

    auto * set_up = new setup;
    set_up->camera = new camera(vec3(13, 2, 3), vec3(0, 0, 0), vec3(0, 1, 0), 20.f, aspect, 0.0f, 0, 1);
    set_up->world = new bvh(world->items, 0, 1);

    return set_up;
  }

  setup* two_perlin_spheres(float aspect)
  {
    texture * noise = new noise_texture(5.f);

    auto * world = new intersectable_container();
    world->items.push_back(new sphere(vec3(0, -1000, 0), 1000, new lambertian(noise)));
    world->items.push_back(new sphere(vec3(0, 2, 0), 2, new lambertian(noise)));

    auto * set_up = new setup;
    set_up->camera = new camera(vec3(13, 2, 3), vec3(0, 0, 0), vec3(0, 1, 0), 20.f, aspect, 0.0f, 0, 1);
    set_up->world = new bvh(world->items, 0, 1);

    return set_up;
  }

  setup* two_spheres_earth(float aspect)
  {
    int w, h, nn;

    unsigned char* tex_data = stbi_load("earth.jpg", &w, &h, &nn, 3);
    assert(tex_data);
    texture * map = new image_texture(tex_data, w, h);
    texture * noise = new noise_texture(5.f);

    auto * world = new intersectable_container();
    world->items.push_back(new sphere(vec3(0, -1001, 0), 1000, new lambertian(noise)));
    world->items.push_back(new sphere(vec3(0, 0, 0), 1, new lambertian(map)));

    auto * set_up = new setup;
    set_up->camera = new camera(vec3(8, 0, 3), vec3(0, 0, 0), vec3(0, 1, 0), 20.f, aspect, 0.0f, 0, 1);
    set_up->world = new bvh(world->items, 0, 1);

    return set_up;
  }

  setup* simple_light(float aspect)
  {
    texture* noise = new noise_texture(5.f);

    texture* light = new constant_texture(vec3(4));

    auto * world = new intersectable_container();
    world->items.push_back(new sphere(vec3(0, -1000, 0), 1000, new lambertian(noise)));
    world->items.push_back(new sphere(vec3(0, 2, 0), 2, new lambertian(noise)));
    world->items.push_back(new sphere(vec3(0, 7, 0), 2, new diffuse_light(light)));
    world->items.push_back(new xy_rect(vec2(3, 1), vec2(5, 3), -2, new diffuse_light(light)));

    auto * set_up = new setup;
    set_up->camera = new camera(vec3(13, 4, 3), vec3(0, 2, 0), vec3(0, 1, 0), 30.f, aspect, 0.0f, 0, 1);
    set_up->world = new bvh(world->items, 0, 1);

    return set_up;
  }

  setup* cornell_box(float aspect)
  {
    auto * red = new lambertian(new constant_texture(vec3(0.65f, 0.05f, 0.05f)));
    auto * white = new lambertian(new constant_texture(vec3(0.73f, 0.73f, 0.73f)));
    auto * green = new lambertian(new constant_texture(vec3(0.12f, 0.45f, 0.15f)));
    auto * light = new diffuse_light(new constant_texture(vec3(15)));

    auto * world = new intersectable_container();
    world->items.push_back(new normal_flip(new yz_rect(vec2(0, 0), vec2(555, 555), 555, green)));
    world->items.push_back(new yz_rect(vec2(0, 0), vec2(555, 555), 0, red));
    world->items.push_back(new xz_rect(vec2(213, 227), vec2(343, 332), 554, light));
    world->items.push_back(new xz_rect(vec2(0, 0), vec2(555, 555), 0, white));
    world->items.push_back(new normal_flip(new xz_rect(vec2(0, 0), vec2(555, 555), 555, white)));
    world->items.push_back(new normal_flip(new xy_rect(vec2(0, 0), vec2(555, 555), 555, white)));

    world->items.push_back(new translate(new rotate_y(new box(vec3(0), vec3(165, 165, 165), white), -18), vec3(130, 0, 65)));
    world->items.push_back(new translate(new rotate_y(new box(vec3(0), vec3(165, 330, 165), white), 15), vec3(265, 0, 295)));

    auto * set_up = new setup;
    set_up->camera = new camera(vec3(278, 278, -800), vec3(278, 278, 0), vec3(0, 1, 0), 40.f, aspect, 0.0f, 0, 1);
    set_up->camera->orientation = axisAngle(vec3(0, 1, 0), float(3.14159265358979323846264338327950288)); // exactly 180 deg rotation not handled yet.
    set_up->world = new bvh(world->items, 0, 1);

    return set_up;
  }

  void renderLine(uint8_t* image, unsigned w, unsigned h, unsigned s, const setup* set_up , unsigned j)
  {
    for (unsigned i = 0; i < w; i++) {

      vec3 col;
      for (unsigned k = 0; k < s; k++) {
        auto u = float(i + frand()) / float(w);
        auto v = float(j + frand()) / float(h);

        auto r = set_up->camera->getRay(u, v);

        col = col + color(r, set_up->world, 20);
      }
      col = (1.f / s)*col;

      image[3 * (j*w + i) + 0] = uint8_t(255.f*saturate(std::sqrt(col.r)));
      image[3 * (j*w + i) + 1] = uint8_t(255.f*saturate(std::sqrt(col.g)));
      image[3 * (j*w + i) + 2] = uint8_t(255.f*saturate(std::sqrt(col.b)));
    }

  }


}

int main(int argc, char** argv)
{
  const char* filename = "output.png";
  const unsigned w = 600;
  const unsigned h = 300;
  const unsigned s = 100;

  uint8_t image[3 * w * h];

  auto start = std::chrono::steady_clock::now();

  //auto * setup = create_world_random(float(w) / float(h));
  //auto * setup = two_spheres(float(w) / float(h));
  //auto * setup = two_perlin_spheres(float(w) / float(h));
  //auto * setup = two_spheres_earth(float(w) / float(h));
  //auto * setup = simple_light(float(w) / float(h));
  auto * setup = cornell_box(float(w) / float(h));

  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);
  fprintf(stderr, "Setup elapsed time %f\n", (1.0 / 1000.0)*duration.count());


  start = std::chrono::steady_clock::now();

  auto T = std::thread::hardware_concurrency();

  auto f = [&](unsigned o) {
    for (unsigned j = o; j < h; j += T) {
      renderLine(image, w, h, s, setup, j);
    }
  };


  std::vector<std::thread> threads;
  for (unsigned o = 1; o < T; o++) {
    threads.emplace_back(std::thread(f, o));
  }

  for (unsigned j = 0; j < h; j+=T) {
    renderLine(image, w, h, s, setup, j);
    fprintf(stderr, "%d of %d\n", j, h);
  }

  for (auto & t : threads) t.join();

  duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);

  fprintf(stderr, "Render elapsed time %f\n", (1.0 / 1000.0)*duration.count());

  stbi_flip_vertically_on_write(1);
  if (stbi_write_png(filename, w, h, 3, image, 3 * w) == 0) {
    fprintf(stderr, "Failed to write png file '%s'", filename);
    return -1;
  }


  return 0;
}