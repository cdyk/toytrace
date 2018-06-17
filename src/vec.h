#pragma once

template<unsigned DIM>
struct vec_base
{
  vec_base operator+(const vec_base& b) const
  {
    vec_base rv;
    for (unsigned i = 0; i < DIM; i++) {
      rv.data[i] = data[i] + b.data[i];
    }
    return rv;
  }

  vec_base operator-(const vec_base& b) const
  {
    vec_base rv;
    for (unsigned i = 0; i < DIM; i++) {
      rv.data[i] = data[i] - b.data[i];
    }
    return rv;
  }

  vec_base operator*(const vec_base& b) const
  {
    vec_base rv;
    for (unsigned i = 0; i < DIM; i++) {
      rv.data[i] = data[i] * b.data[i];
    }
    return rv;
  }


  const float& operator[](unsigned i) const { return data[i]; }
  float& operator[](unsigned i) { return data[i]; }

  union {
    float data[DIM];
    struct {
      float x;
      float y;
      float z;
      float w;
    };
  };
};

template<unsigned DIM> inline vec_base<DIM> operator*(const float a, const vec_base<DIM>& b)
{
  vec_base rv;
  for (unsigned i = 0; i < DIM; i++) {
    rv.data[i] = a * b.data[i];
  }
  return rv;
}

template<unsigned DIM> float dot(const vec_base<DIM>& a, const vec_base<DIM>& b)
{
  float rv = a[0]*b[0];
  for (unsigned i = 1; i < DIM; i++) {
    rv += a[i] * b[i];
  }
  return rv;
}

struct vec2 : public vec_base<2>
{
  vec2() { data[0] = 0.f; data[1] = 0.f; }
  vec2(float e0, float e1) { data[0] = e0; data[1] = e1; }

};

struct vec3 : public vec_base<3>
{
  vec3() { data[0] = 0.f; data[1] = 0.f; data[2] = 0.f; }
  vec3(float e0, float e1, float e2) { data[0] = e0; data[1] = e1; data[2] = e2; }
};

struct vec4 : public vec_base<4>
{
  vec4() { data[0] = 0.f; data[1] = 0.f; data[2] = 0.f; data[3] = 1.f; }
  vec4(float e0, float e1, float e2, float e3) { data[0] = e0; data[1] = e1; data[2] = e2; data[3] = e3; }
};

vec3 cross(const vec_base<3>& a, const vec_base<3>& b)
{
  return vec3(a[1] * b[2] - a[2] * b[1],
              a[2] * b[0] - a[0] * b[2],
              a[0] * b[1] - a[1] * b[0]);
}
