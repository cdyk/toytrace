#include "stb_image_write.h"
#include <cstdint>
#include <cstdio>
#include "vec.h"

int main(int argc, char** argv)
{
  const char* filename = "output.png";
  const unsigned w = 200;
  const unsigned h = 100;

  uint8_t image[3 * w * h];

  for (unsigned j = 0; j < h; j++) {
    for (unsigned i = 0; i < w; i++) {
      image[3 * (j*w + i) + 0] = i;
      image[3 * (j*w + i) + 1] = j;
      image[3 * (j*w + i) + 2] = 0;
    }
  }


  vec3 a, b;

  auto t = a - b;

  if (stbi_write_png(filename, w, h, 3, image, 3*w)) {
    fprintf(stderr, "Failed to write png file '%s'", filename);
    return -1;
  }

  return 0;
}