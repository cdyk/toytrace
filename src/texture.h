#pragma once
#include "vec3.h"

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