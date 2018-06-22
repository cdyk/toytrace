#pragma once

struct vec2
{
  vec2() : x(0), y(0) {}

  vec2(float e) : x(e), y(e) {}

  vec2(float x, float y) : x(x), y(y) { }

  vec2 operator+(const vec2& b) const { return vec2(x + b.x, y + b.y); }

  vec2 operator-(const vec2& b) const { return vec2(x - b.x, y - b.y); }

  vec2 operator-() const { return vec2(-x, -y); }

  vec2 operator*(const vec2& b) const { return vec2(x * b.x, y * b.y); }

  vec2 operator/(const vec2& b) const { return vec2(x / b.x, y / b.y); }

  const float& operator[](unsigned i) const { return data[i]; }

  float& operator[](unsigned i) { return data[i]; }

  union {
    float data[2];
    struct {
      float x;
      float y;
    };
    struct {
      float u;
      float v;
    };
    struct {
      float r;
      float g;
    };
  };
};

inline vec2 operator*(const float a, const vec2& b) { return vec2(a*b.x, a*b.y); }

inline float dot(const vec2& a, const vec2& b) { return a.x*b.x + a.y*b.y; }

float length(const vec2& v);

inline float distance(const vec2& a, const vec2& b) { return length(a - b); }

vec2 normalize(const vec2& x);

inline float cross(const vec2& a, const vec2& b) { return a.x * b.y - a.y * b.x; }

inline vec2 mix(const vec2& a, const vec2& b, const float t) { return (1.f - t) * a + t * b; }
