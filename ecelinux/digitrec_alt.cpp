//==========================================================================
//digitrec_alt.cpp
//==========================================================================
// @brief: A digit recognition system for dice

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>


#include "digitrec.h"
#include "conn_components_alt.h"

//----------------------------------------------------------
// Top function
//----------------------------------------------------------

pixel threshold_value = 107;
buf_bit in_buffer = 0;
buf_8 out_buffer = 0;
buf_8 out_bufferW = 0;
pixel un_classW[256];
pixel un_class[256];
pixel label[256];
bit16_t size[128];
bit4_t dice_value[256];
pixel row_value = 0;
pixel column_value = 0;
void dut(
  hls::stream<bit32_t> &strm_in,
  hls::stream<pixel> &strm_out
)
{

  #pragma HLS array_partition variable=un_classW block factor=4
  #pragma HLS array_partition variable=un_class block factor=4
//  #pragma HLS array_partition variable=label block factor=8
//  #pragma HLS array_partition variable=size
//  #pragma HLS array_partition variable=dice_value

  int N = (ROW*COL)/4;

  for(int m = 0; m<256; m++){
    #pragma HLS unroll
    un_class[m] = 0;
    un_classW[m] = 0;
    label[m] = 0;
    dice_value[m] = 0;
    size[m] = 0;
  }
  for(int j = 0; j < N; j++){
    // Read the two input 32-bit words (low word first)
    bit32_t input_lo = strm_in.read();
    // Update the histogram
    bit4_t threshold_bit;

    for(int i = 3; i >= 0; i--){
      #pragma HLS unroll
      pixel chunk = input_lo((i << 3) + 7, (i << 3));
      threshold_bit[i] = (chunk >= threshold_value);
    } 
    pixel connected_c;
    pixel connected_cW;

    for (int i = 3; i >= 0; i--){
      in_buffer(COL+1,1) = in_buffer(COL,0);
      in_buffer[0] = threshold_bit[i];
      out_buffer((COL+1)*8 + 7,8) = out_buffer((COL)*8 + 7,0);
      out_bufferW((COL+1)*8 + 7,8) = out_bufferW((COL)*8 + 7,0);
      connected_cW = conn_comp_1st_pass_white(in_buffer, &out_bufferW, un_classW, COL, ROW, column_value, row_value);
      out_bufferW(7,0) = connected_cW;
      connected_c = conn_comp_1st_pass_black(in_buffer, &out_buffer, un_class, COL, ROW, column_value, row_value, label, out_bufferW);
      out_buffer(7,0) = connected_c;
      size[connected_c] +=1;
      column_value += 1;
      if (column_value >= COL) {
        row_value+=1;
        column_value = 0;
      }
    }
  }
  int black_dots = 0;
  for (int k= 255; k>=0; k--){
    int add = 1;
    int add2 = 1;
    int name = un_class[k];
    //printf("iteration: %d black name : %d\n", k, name);
    for (int m= 0; m<255; m++){
      if(name == un_class[m] && m<k){
        add = 0;
        if(add2){
          size[m] += size[k];
          add2 = 0;
        }
      }
    }
    if(add){
      black_dots++;
      bool cont = size[k]>10 && size[k]<100;
      pixel next_W = un_classW[label[name]];
      if(next_W != 0 && cont){
        dice_value[next_W] +=1;
      }
    }
  }
  int count = 0;
  for (int l = 0; l<256; l++){
    if(dice_value[l]>0){
      bit32_t output = 0;
      output(3, 0) = dice_value[l];
      strm_out.write(output);
      count++;
    }
  }
}