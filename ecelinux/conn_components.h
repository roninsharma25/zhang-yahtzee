#ifndef NUM_PIXELS
#define NUM_PIXELS 11200
#endif
#include "typedefs.h"
bit3_t conn_comp_1st_pass( buf_bit in_buffer, buf_3 out_buffer, bit3_t un_class[20], int width, int height, int x, int y);
void conn_comp_2nd_pass(int input_image[NUM_PIXELS], int output_image[NUM_PIXELS], int* un_class, int width, int height);
