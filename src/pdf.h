#pragma once

#include "vec3.h"

class intersectable;

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

class hitable_pdf : public pdf
{
public:
  hitable_pdf() = delete;

  hitable_pdf(const intersectable* object, const vec3& origin);

  virtual float value(const vec3& direction) const override;

  virtual vec3 generate() const override;

  const intersectable* object;
  vec3 origin;
};

class mixture_pdf : public pdf
{
public:
  mixture_pdf(const pdf* a, const pdf* b);

  virtual float value(const vec3& direction) const override;

  virtual vec3 generate() const override;

  const pdf* a;
  const pdf* b;
};
