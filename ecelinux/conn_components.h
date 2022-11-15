#ifndef NUM_PIXELS
#define NUM_PIXELS 168100
#endif
#include "typedefs.h"
pixel conn_comp_1st_pass( buf_bit in_buffer, buf_8 out_buffer, pixel un_class[256], int width, int height, int x, int y);
void conn_comp_2nd_pass(int input_image[NUM_PIXELS], int output_image[NUM_PIXELS], int* un_class, int width, int height);
int un_class_method(bit A, bit B, bit C, bit D, pixel labelA, pixel labelB, pixel labelC, pixel labelD, pixel un_class[256]);