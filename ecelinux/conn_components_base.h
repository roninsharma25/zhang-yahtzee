
#include "typedefs.h"

pixel conn_comp_1st_pass_black( base_buf_bit in_buffer, base_buf_8 *out_buffer, pixel un_class[256], int width, int height, int x, int y, pixel label[256], base_buf_8 out_bufferW);

pixel conn_comp_1st_pass_white( base_buf_bit in_buffer, base_buf_8 *out_buffer, pixel un_class[256], int width, int height, int x, int y);

int un_class_method(bit A, bit B, bit C, bit D, pixel labelA, pixel labelB, pixel labelC, pixel labelD, pixel un_class[256], bit type);
