
#ifndef NUM_PIXELS
#define NUM_PIXELS 15
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

// void threshold_image(hls::stream<pixel> &input, hls::stream<bit> &output,
//  pixel threshold);