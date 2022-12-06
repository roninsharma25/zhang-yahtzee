//==========================================================================
//dummy.cpp
//==========================================================================
// @brief: A dummy kernel to test memory bandwidth.
#include "dummy.h"

//----------------------------------------------------------
// Top function
//----------------------------------------------------------



void dut(
    hls::stream<bit32_t> &strm_in,
    hls::stream<bit32_t> &strm_out
)
{
bit32_t sum;
  for (int i = 0; i < PIXELS; i++){
      bit32_t input_0 = strm_in.read();
      sum = sum + input_0(7,0) + input_0(15,8) + input_0(23, 16) + input_0(31, 24);
  }

  for (int i = 0; i < NUMDIE; i++){
    strm_out.write(sum);
  }
}