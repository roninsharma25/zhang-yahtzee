//===========================================================================
// typedefs.h
//===========================================================================
// @brief: This header defines the shorthand of several ap_uint data types.

#ifndef TYPEDEFS_H
#define TYPEDEFS_H
#define AP_INT_MAX_W 4000
#define COL 210//410
#define ROW 210

#include "ap_int.h"

typedef ap_uint<3> bit3_t;
typedef ap_uint<4> bit4_t;
typedef ap_uint<6> bit6_t;
typedef ap_uint<16> bit16_t;
typedef ap_uint<32> bit32_t;
typedef ap_uint<64> bit64_t;
typedef ap_uint<49> digit;

typedef ap_uint<8> pixel;
typedef ap_uint<1> bit;
typedef ap_uint<COL+5> buf_bit;
typedef ap_uint<(COL+5)*8> buf_8;
typedef ap_uint<(COL+2)*6> buf_6;
typedef ap_uint<(COL+2)*3> buf_3;

#endif
