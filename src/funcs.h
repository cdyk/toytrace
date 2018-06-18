#pragma once

float saturate(const float x)
{
  return x < 0.f ? 0.f : (1.f < x ? 1.f : x);
}

float radians(const float deg)
{
  constexpr float s = float(3.14159265358979323846264338327950288 / 180.0);
  return deg * s;
}
