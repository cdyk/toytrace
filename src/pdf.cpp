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
