//==========================================================================
//digitrec.cpp
//==========================================================================
// @brief: A k-nearest-neighbor implementation for digit recognition (k=1)
//data flow pipeline

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>


#include "digitrec.h"
#include "otsu.h"
#include "conn_components_alt.h"

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
buf_8 out_bufferW = 0;
pixel un_classW[256];
pixel un_class[256];
pixel label[256];
int size[256];
int dice_value[256];
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
  if(rows == 0 && cols == 0){
    in_buffer = 0;
    out_buffer = 0;
    zero_n = 0;
    row_value = 0;
    column_value = 0;
    first = 1;
    count = 0;
    otsu_mode = 1;
  }
  int pixels = rows * cols;
  int N = (ROW*COL)/4;
  if(otsu_mode){
    for (int i = 0; i < 256; i++){
      histogram[i] = 0;
    }

    for(int i = 0; i < N; i++){
      // Read the two input 32-bit words (low word first)
        bit32_t input_lo = strm_in.read();
        // Update the histogram
        update_histogram(input_lo, histogram);
    }
    threshold_value = 107; //otsu(histogram);
    //printf("threshold value: %d\n", threshold_value.to_int());
    strm_out.write(threshold_value);
  } else {
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
      int not_one = 1;
      int not_zero = 1; 
      for(int i = 3; i >= 0; i--){
        pixel chunk = input_lo((i << 3) + 7, (i << 3));
        threshold_bit[i] = threshold_image(chunk, threshold_value);
        if (threshold_bit[i] == 0) not_one = 0;
        if (threshold_bit[i] == 1) not_zero = 0; 
      } 
      pixel connected_c;
      pixel connected_cW;
      int out_c;
      //printf("%d Start: one: %d zero: %d \n", j, not_one, not_zero);
      if ((threshold_bit == 15 || threshold_bit == 0) && column_value < COL - 4){
        //printf("HERE\n");
        out_buffer((COL+1)*8 + 7,8) = out_buffer((COL)*8 + 7,0);
        out_bufferW((COL+1)*8 + 7,8) = out_bufferW((COL)*8 + 7,0);
        in_buffer(COL+1,1) = in_buffer(COL,0);     
        in_buffer[0] = threshold_bit[3]; 
        connected_cW = conn_comp_1st_pass_white(in_buffer, &out_bufferW, un_classW, COL, ROW, column_value, row_value);
        out_bufferW(7,0) = connected_cW;
        connected_c = conn_comp_1st_pass_black(in_buffer, &out_buffer, un_class, COL, ROW, column_value, row_value, label, out_bufferW);
        out_buffer(7,0) = connected_c;
        //std::cout << "white " << connected_cW << std::endl;
        out_bufferW((COL+3)*8 + 7, 24) = out_bufferW((COL)*8 + 7,0);
        out_buffer((COL+3)*8 + 7, 24) = out_buffer((COL)*8 + 7, 0); 
        for (int i = 2; i >= 0; i--){
          //out_bufferW((COL+1)*8 + 7,8) = out_bufferW((COL)*8 + 7,0);
          //out_buffer((COL+1)*8 + 7,8) = out_buffer((COL)*8 + 7,0);
          out_bufferW(i*8 + 7,i*8) = connected_cW;
          out_buffer(i*8 + 7,i*8) = connected_c;
        }
        in_buffer(COL+3, 3) = in_buffer(COL, 0);
        in_buffer(2,0) = threshold_bit(2,0);       
        size[connected_c] +=4;
        if ((threshold_bit == 15)) link_pixel(in_buffer, &out_bufferW, un_classW, COL, connected_cW, 1);
        if ((threshold_bit == 0)) link_pixel(in_buffer, &out_buffer, un_class, COL, connected_c, 0);
        column_value += 4;     
      }
      else {
        for (int i = 3; i >= 0; i--){
          in_buffer(COL+1,1) = in_buffer(COL,0);
          in_buffer[0] = threshold_bit[i];
          out_buffer((COL+1)*8 + 7,8) = out_buffer((COL)*8 + 7,0);
          out_bufferW((COL+1)*8 + 7,8) = out_bufferW((COL)*8 + 7,0);
          connected_cW = conn_comp_1st_pass_white(in_buffer, &out_bufferW, un_classW, COL, ROW, column_value, row_value);
          out_bufferW(7,0) = connected_cW;
          //std::cout << "white " << connected_cW << std::endl;
          connected_c = conn_comp_1st_pass_black(in_buffer, &out_buffer, un_class, COL, ROW, column_value, row_value, label, out_bufferW);
          out_buffer(7,0) = connected_c;
          //std::cout << "black " << connected_c << std::endl;
          size[connected_c] +=1;
          column_value += 1;
          if (column_value >= COL) {
            row_value+=1;
            column_value = 0;
          }
        }
      }
        // Connected components     
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
        //std::cout << "black dots" << black_dots << std::endl;
        //printf("black dot size %d\n", size[k]);
        bool cont = size[k]>10 && size[k]<100;
        pixel next_W = un_classW[label[name]];
        //std::cout << "label " << label[name] << std::endl; 
        //std::cout << "next_W " << next_W << std::endl;
        if(next_W != 0 && cont){
          dice_value[next_W] +=1;
          //printf("dice_value: %d\n", dice_value[next_W]);
        }
      }
    }
    //printf("black dots %d", black_dots);
    int count = 0;
    for (int l = 0; l<256; l++){
      if(dice_value[l]>0){
        strm_out.write(dice_value[l]);
        count++;
      }
    }
  }
}