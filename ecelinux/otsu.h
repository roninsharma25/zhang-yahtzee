
#ifndef NUM_PIXELS
#define NUM_PIXELS 11200
#endif

#ifndef ROW
#define ROW 100
#endif

#include "typedefs.h"

pixel otsu(int histogram[]);

int sum(int a, int b);

void update_histogram(bit32_t pixel_value, int histogram[]);

bit threshold_image(pixel input, pixel threshold_input);
