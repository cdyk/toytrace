#include "intersectable.h"
#include "vec3.h"
#include "funcs.h"
#include "pdf.h"

cosine_pdf::cosine_pdf(const vec3& d)
{
  orthonormal(u, v, w, d);
}

float cosine_pdf::value(const vec3& direction) const
{
  auto cosine = dot(normalize(direction), w);
  return mmax(0.f, cosine)*one_over_pi;
}

vec3 cosine_pdf::generate() const
{
  auto l = random_cosine_direction();
  return l.x*u + l.y*v + l.z*w;
}

hitable_pdf::hitable_pdf(intersectable* object, const vec3& origin) :
  object(object),
  origin(origin)
{
}

float hitable_pdf::value(const vec3& direction) const
{
  return object->pdf_value(origin, direction);
}

vec3 hitable_pdf::generate() const
{
  return object->random(origin);
}

mixture_pdf::mixture_pdf(pdf* a, pdf* b) :
  a(a),
  b(b)
{}

float mixture_pdf::value(const vec3& direction) const
{
  return 0.5f*a->value(direction) + 0.5f*b->value(direction);
}

vec3 mixture_pdf::generate() const
{
  if (frand() < 0.5f) {
    return a->generate();
  }
  else {
    return b->generate();
  }

}
