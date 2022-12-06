#ifndef NUM_PIXELS
#define NUM_PIXELS 168100
#endif
#include "typedefs.h"

pixel conn_comp_1st_pass_black( buf_bit in_buffer, buf_8 *out_buffer, pixel un_class[256], int width, int height, int x, int y, pixel label[256], buf_8 out_bufferW, pixel *labelNoB);

pixel conn_comp_1st_pass_white( buf_bit in_buffer, buf_8 *out_buffer, pixel un_class[256], int width, int height, int x, int y, pixel *labelNoW);

int un_class_method(bit A, bit B, bit C, bit D, pixel labelA, pixel labelB, pixel labelC, pixel labelD, pixel un_class[256], bit type, pixel *output);

void mergeB( buf_8 maB, buf_8 mbB,buf_bit mid_b, buf_bit mid_a, int size[256], int size2[256] );

void mergeW( buf_8 maW, buf_8 mbW, buf_bit mid_b, buf_bit mid_a, pixel un_classW[256], pixel un_classW2[256]);