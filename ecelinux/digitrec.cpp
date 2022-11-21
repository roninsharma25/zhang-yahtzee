//==========================================================================
//digitrec.cpp
//==========================================================================
// @brief: A k-nearest-neighbor implementation for digit recognition (k=1)

#include "digitrec.h"
#include "otsu.h"
#include "conn_components.h"

//----------------------------------------------------------
// Top function
//----------------------------------------------------------


int first = 1;
int histogram[256];
int count = 0;
int otsu_mode = 1;
pixel threshold_value;
buf_bit in_buffer = 0;
buf_8 out_buffer = 0;
pixel un_class[64];
int zero_n = 0;
int row_value = 0;
int column_value = 0;
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
    // printf("histogram: [");
    // for(int i = 0; i < 256; i++){
    //   printf("%d,", histogram[i]);
    // }
    // printf("]\n");
    threshold_value = otsu(histogram);
    // printf("threshold value: %d\n", threshold_value.to_int());
    strm_out.write(threshold_value);
  } else {
    for(int j = 0; j < 42025; j++){
      // Read the two input 32-bit words (low word first)
      bit32_t input_lo = strm_in.read();
      // Update the histogram
      for(int i = 3; i >= 0; i--){
        pixel chunk = input_lo((i << 3) + 7, (i << 3));
        bit threshold_bit = threshold_image(chunk, threshold_value);
        // strm_out.write(threshold_bit);

        // Connected components
        pixel connected_c;
        in_buffer(COL,1) = in_buffer(COL - 1,0);
        in_buffer[0] = threshold_bit;
        out_buffer((COL)*8 + 7,8) = out_buffer((COL-1)*8 + 7,0);
        connected_c = conn_comp_1st_pass(in_buffer, &out_buffer, un_class, COL, ROW, column_value, row_value);
        //if (connected_c != 0) printf("connected c is %d\n", connected_c.to_int());
        out_buffer(7,0) = connected_c;
        if ((j >= 103) || (j == 102 && i == 0)) strm_out.write(out_buffer((COL)*8 + 7, (COL)*8));
        column_value += 1;
        if (column_value >= COL) {
          row_value+=1;
          column_value = 0;
        }

      }
    }
    for (int j = COL; j >= 0; j--){
      strm_out.write(out_buffer((j)*8 + 7, (j)*8));
    }

  }
}
