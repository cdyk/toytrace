#pragma once
#include <cstdint>

#include "vec3.h"
#include "noise.h"

class texture
{
public:
  virtual vec3 value(float u, float v, const vec3& p) const = 0;
};

class constant_texture : public texture
{
public:
  constant_texture() = delete;

  constant_texture(const vec3& color) : color(color) {}

  virtual vec3 value(float u, float v, const vec3& p) const override { return color; }

  vec3 color;
};

class checker_texture : public texture
{
public:
  checker_texture() = delete;
  checker_texture(texture* odd, texture* even) : odd(odd), even(even) {}

  virtual vec3 value(float u, float v, const vec3& p) const override {
    if (((int)std::floor(3.f*p.x) ^ (int)std::floor(3.f*p.y) ^ (int)std::floor(3.f*p.z)) & 1) return odd->value(u, v, p); else return even->value(u, v, p);
  }

  texture* odd;
  texture* even;
};

class noise_texture : public texture
{
public:
  noise_texture() = delete;

  noise_texture(float scale) : scale(scale) {}

  virtual vec3 value(float u, float v, const vec3& p) const override {
    //return 0.5f*(perlin::noise(scale * p) + 1.f) * vec3(1, 1, 1);
    //return perlin::turbulence(scale * p) * vec3(1,1,1);
    return 0.5f*(1 + std::sin(scale*p.z + 10.f*perlin::turbulence(p)));
  }

  float scale;
};

class image_texture : public texture
{
public:
  image_texture() = delete;

  image_texture(uint8_t* pixels_rgb, unsigned w, unsigned h) : pixels_rgb(pixels_rgb), w(w), h(h) {}

  virtual vec3 value(float u, float v, const vec3& p) const override
  {
    u = u < 0.f ? 0.f : u;
    v = v < 0.f ? 0.f : v;
    auto i = unsigned(u*w);
    i = i < w ? i : w - 1;
    auto j = unsigned(v*h);
    j = j < h ? j : h - 1;

    auto * rgb = pixels_rgb + 3 * (j*w + i);
    constexpr auto s = 1.f / 255.f;
    return vec3(s*rgb[0], s*rgb[1], s*rgb[2]);
  }

  uint8_t * pixels_rgb;
  unsigned w;
  unsigned h;
};

class image_texture_spheremap : public image_texture
{
public:
  image_texture_spheremap(uint8_t* pixels_rgb, unsigned w, unsigned h) : image_texture(pixels_rgb, w, h) {}

  virtual vec3 value(float u, float v, const vec3& p) const override
  {
    constexpr float pi = float(3.14159265358979323846264338327950288);
    constexpr float pi_over_two = float(3.14159265358979323846264338327950288/2.0);
    constexpr float one_over_pi = float(1.0 / (3.14159265358979323846264338327950288));
    constexpr float one_over_two_pi = float(1.0 / (2 * 3.14159265358979323846264338327950288));

    float phi = atan2(p.z, p.x);
    float theta = asin(p.y);
    return image_texture::value(1.f - one_over_two_pi * (phi + pi),
                                1.f - one_over_pi*(theta + pi_over_two),
                                p);
  }


};
