#pragma once

float saturate(const float x)
{
  return x < 0.f ? 0.f : (1.f < x ? 1.f : x);
}