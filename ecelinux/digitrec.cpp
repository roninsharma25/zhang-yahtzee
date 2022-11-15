//==========================================================================
//digitrec.cpp
//==========================================================================
// @brief: A k-nearest-neighbor implementation for digit recognition (k=1)

#include "digitrec.h"
#include "otsu.h"

//----------------------------------------------------------
// Top function
//----------------------------------------------------------

int first = 1;
int histogram[256];
int count = 0;
int otsu_mode = 1;
pixel threshold_value;

void dut(
    hls::stream<bit32_t> &strm_in,
    hls::stream<pixel> &strm_out,
    int rows,
    int cols,
    int otsu_mode
)
{
  int pixels = rows * cols;

  if(otsu_mode){
    for (int i = 0; i < 256; i++){
      histogram[i] = 0;
    }

    for(int i = 0; i < 42025; i++){
      // Read the two input 32-bit words (low word first)
        bit32_t input_lo = strm_in.read();
        // Update the histogram
        update_histogram(input_lo, histogram);
    }

    printf("histogram: [");
    for(int i = 0; i < 256; i++){
      printf("%d,", histogram[i]);
    }
    printf("]\n");
    threshold_value = otsu(histogram);
    printf("threshold value: %d\n", threshold_value.to_int());
    strm_out.write(threshold_value);
  } else {
    for(int i = 0; i < 42025; i++){
      // Read the two input 32-bit words (low word first)
      bit32_t input_lo = strm_in.read();
      // Update the histogram
      bit threshold_bit;
      for(int i = 0; i < 4; i++){
        threshold_bit = threshold_image(input_lo, threshold_value);
        strm_out.write(threshold_bit);
      }
    }
  }
}
