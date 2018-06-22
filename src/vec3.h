#pragma once

struct vec3
{
  vec3() { data[0] = 0.f; data[1] = 0.f; data[2] = 0.f; }

  vec3(float e) : x(e), y(e), z(e) {}

  vec3(float e0, float e1, float e2) { data[0] = e0; data[1] = e1; data[2] = e2; }

  vec3 operator+(const vec3& b) const { return vec3(x + b.x, y + b.y, z + b.z); }

  vec3 operator-(const vec3& b) const { return vec3(x - b.x, y - b.y, z - b.z); }

  vec3 operator-() const { return vec3(-x, -y, -z); }

  vec3 operator*(const vec3& b) const { return vec3(x * b.x, y * b.y, z * b.z); }

  vec3 operator/(const vec3& b) const { return vec3(x / b.x, y / b.y, z / b.z); }

  const float& operator[](unsigned i) const { return data[i]; }

  float& operator[](unsigned i) { return data[i]; }

  union {
    float data[3];
    struct {
      float x;
      float y;
      float z;
    };
    struct {
      float r;
      float g;
      float b;
    };
  };
};

inline vec3 operator*(const float a, const vec3& b) { return vec3(a*b.x, a*b.y, a*b.z); }

inline float dot(const vec3& a, const vec3& b) { return a.x*b.x + a.y*b.y + a.z*b.z; }

float length(const vec3& v);

inline float distance(const vec3& a, const vec3& b) { return length(a - b); }

// v points towards surface, results points away.
inline vec3 reflect(const vec3& v, const vec3& n) { return v - 2 * dot(v, n)*n; }

vec3 normalize(const vec3& x);

inline vec3 cross(const vec3& a, const vec3& b)
{
  return vec3(a.y * b.z - a.z * b.y,
              a.z * b.x - a.x * b.z,
              a.x * b.y - a.y * b.x);
}

inline vec3 mix(const vec3& a, const vec3& b, const float t) { return (1.f - t) * a + t * b; }