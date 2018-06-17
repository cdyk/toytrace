#pragma once
#include "vec.h"

template<unsigned COLS, unsigned ROWS>
struct mat_base
{


  union {
    float data[COLS*ROWS];
    vec_base<ROWS> col[COLS];
  };
};

struct mat3 : public mat_base<3, 3>
{
  mat3()
  {
    data[0] = 1.f; data[1] = 0.f; data[2] = 0.f;
    data[3] = 0.f; data[4] = 1.f; data[5] = 0.f;
    data[6] = 0.f; data[7] = 0.f; data[8] = 1.f;
  }

  mat3(float c0r0, float c0r1, float c0r2,
       float c1r0, float c1r1, float c1r2,
       float c2r0, float c2r1, float c2r2)
  {
    data[0] = c0r0; data[1] = c0r1; data[2] = c0r2;
    data[3] = c1r0; data[4] = c1r1; data[5] = c1r2;
    data[6] = c2r0; data[7] = c2r1; data[8] = c2r2;
  }

};

struct mat4 : public mat_base<4, 4>
{
  mat4()
  {
    data[ 0] = 1.f; data[ 1] = 0.f; data[2] =  0.f; data[ 3] = 0.f;
    data[ 4] = 0.f; data[ 5] = 1.f; data[6] =  0.f; data[ 7] = 0.f;
    data[ 8] = 0.f; data[ 9] = 0.f; data[10] = 1.f; data[11] = 0.f;
    data[12] = 0.f; data[13] = 0.f; data[14] = 0.f; data[15] = 1.f;
  }

  mat4(float c0r0, float c0r1, float c0r2, float c0r3,
       float c1r0, float c1r1, float c1r2, float c1r3,
       float c2r0, float c2r1, float c2r2, float c2r3,
       float c3r0, float c3r1, float c3r2, float c3r3)
  {
    data[ 0] = c0r0; data[ 1] = c0r1; data[ 2] = c0r2; data[ 3] = c0r3;
    data[ 4] = c1r0; data[ 5] = c1r1; data[ 6] = c1r2; data[ 7] = c1r3;
    data[ 8] = c2r0; data[ 9] = c2r1; data[10] = c2r2; data[11] = c2r3;
    data[12] = c3r0; data[13] = c3r1; data[14] = c3r2; data[15] = c3r3;
  }

};