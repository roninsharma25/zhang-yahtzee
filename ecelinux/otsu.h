
#ifndef NUM_PIXELS
#define NUM_PIXELS 168100
#endif

#ifndef ROW
#define ROW 5
#endif

#ifndef COL
#define COL 3
#endif

#include "typedefs.h"

pixel otsu(int histogram[]);

void update_histogram(bit32_t pixel_value, int histogram[]);

bit threshold_image(pixel input, pixel threshold_input);
