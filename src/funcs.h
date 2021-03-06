#pragma once

struct vec3;

constexpr float pi            = float(3.14159265358979323846264338327950288);
constexpr float two_pi        = float(2.0 * 3.14159265358979323846264338327950288);
constexpr float one_over_pi   = float(1.0 / 3.14159265358979323846264338327950288);
constexpr float max_float     = float(3.402823466e+38F);

inline float saturate(const float x)
{
  return x < 0.f ? 0.f : (1.f < x ? 1.f : x);
}

inline float denan(const float x)
{
  return x == x ? x : 0.f;
}

inline float radians(const float deg)
{
  constexpr float s = float(3.14159265358979323846264338327950288 / 180.0);
  return deg * s;
}

// Compiles to minss with msvc /O2
inline float mmin(float x, float y)
{
  return x < y ? x : y;
}

// Compiles to maxss with msvc /O2
inline float mmax(float x, float y)
{
  return x < y ? y : x;
}


float frand();

vec3 random_in_unit_disc();

vec3 random_in_unit_sphere();

// p(dir) = cos(theta)/Pi.
vec3 random_cosine_direction();

// create orthonormal basis where w is along d.
void orthonormal(vec3& u, vec3& v, vec3& w, const vec3& d);

bool refract(vec3& refracted, const vec3& v, const vec3& n, float ni_over_nt);

float schlick(float cosine, float ref_idx);
