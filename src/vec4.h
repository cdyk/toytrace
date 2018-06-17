#pragma once

struct vec4
{
  vec4() : x(0.f), y(0.f), z(0.f), w(1.f) { }

  vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { }

  vec4 operator+(const vec4& b) const { return vec4(x + b.x, y + b.y, z + b.z, w + b.w); }

  vec4 operator-(const vec4& b) const { return vec4(x - b.x, y - b.y, z - b.z, w - b.w); }

  vec4 operator*(const vec4& b) const { return vec4(x * b.x, y * b.y, z * b.z, w * b.w); }

  const float& operator[](unsigned i) const { return data[i]; }

  float& operator[](unsigned i) { return data[i]; }

  union {
    float data[4];
    struct {
      float x;
      float y;
      float z;
      float w;
    };
  };
};

inline vec4 operator*(const float a, const vec4& b) { return vec4(a*b.x, a*b.y, a*b.z, a*b.w); }

inline float dot(const vec4& a, const vec4& b) { return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w; }

