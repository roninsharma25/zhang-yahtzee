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
    hls::stream<pixel> &strm_out
)
{
  // -----------------------------
  // YOUR CODE GOES HERE
  // -----------------------------

  bit32_t test_digit;
  // Read the two input 32-bit words (low word first)
  bit32_t input_lo = strm_in.read();
  // Set digit properly
  test_digit(31, 0) = input_lo;
  if(otsu_mode){
    if(first){
      for (int i = 0; i < 256; i++){
        histogram[i] = 0;
      }
      first = 0;
    }

    // Update the histogram
    update_histogram(test_digit, histogram);
    
    count++;

    // Write out the interpreted digit
    if(count >= 2800){
      printf("histogram: [");
        for(int i = 0; i < 256; i++){
          printf("%d,", histogram[i]);
        }
      printf("]\n");
      threshold_value = otsu(histogram);
      printf("threshold value: %d\n", threshold_value.to_int());
      strm_out.write(threshold_value);
      otsu_mode = 0;
    }
  } else {
    bit threshold_bit;
    for(int i = 0; i < 4; i++){
      threshold_bit = threshold_image(input_lo, threshold_value);
      strm_out.write(threshold_bit);
    }
  }
  


}
