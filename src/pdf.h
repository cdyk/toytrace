#pragma once

#include "vec3.h"

class pdf
{
public:

  virtual float value(const vec3& direction) const = 0;

  virtual float one_over_value(const vec3& direction) const { return 1.f / value(direction); }

  virtual vec3 generate() const = 0;

};

class cosine_pdf : public pdf
{
public:
  cosine_pdf() = delete;

  cosine_pdf(const vec3& d);

  virtual float value(const vec3& direction) const override;

  virtual vec3 generate() const override;

  vec3 u, v, w;

};
