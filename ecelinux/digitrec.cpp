//==========================================================================
//digitrec.cpp
//==========================================================================
// @brief: A k-nearest-neighbor implementation for digit recognition (k=1)
//data flow pipeline
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
buf_bit in_buffer2 = 0;
buf_8 out_buffer = 0;
buf_8 out_bufferW = 0;
buf_8 out_buffer2 = 0;
buf_8 out_bufferW2 = 0;
pixel un_classW[256];
pixel un_class[256];
pixel label[256];
pixel un_classW2[256];
pixel un_class2[256];
pixel label2[256];
int size[256];
int size2[256];
int dice_value[256];
int zero_n = 0;
int row_value = 0;
int row_value2 = 0;
int column_value = 0;
int column_value2 = 0;
pixel labelNoB =1;
pixel labelNoW =1;
pixel labelNoB2 =1;
pixel labelNoW2 =128;
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
    in_buffer2 = 0;
    //out_buffer = 0;
    pixel labelNoB =1;
    pixel labelNoW =1;
    pixel labelNoB2 =1;
    pixel labelNoW2 =128;
    zero_n = 0;
    row_value = 0;
    row_value2 = 205;
    column_value = 0;
    column_value2 = 0;
    first = 1;
    count = 0;
    otsu_mode = 1;
  }
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
    threshold_value = otsu(histogram);
    //printf("threshold value: %d\n", threshold_value.to_int());
    strm_out.write(threshold_value);
  } else {
    buf_8 mid_bB = 0;
    //buf_8 mid_aB = 0;
    buf_8 mid_bW = 0;
    buf_bit mid_bval = 0;
    buf_8 check = 0;
    //buf_8 mid_aW = 0;
    for(int m = 0; m<256; m++){
      un_class[m] = 0;
      un_classW[m] = 0;
      label[m] = 0;
      dice_value[m] = 0;
      size[m] = 0;
      un_class2[m] = 0;
      un_classW2[m] = 0;
      label2[m] = 0;
      size2[m] = 0;
    }
    for(int j = 0; j < 42025; j++){
      #pragma unroll
      if(j == 206){
        mid_bB = out_buffer2;
        mid_bW = out_bufferW2;
        mid_bval = in_buffer2;  
      }
      // Read the two input 32-bit words (low word first)
      bit32_t input_lo = strm_in.read();
      // Update the histogram
      for(int i = 3; i >= 2; i--){
        #pragma unroll
        pixel chunk = input_lo((i << 3) + 7, (i << 3));
        bit threshold_bit = threshold_image(chunk, threshold_value);

        // Connected components
        pixel connected_c;
        pixel connected_cW;
        int out_c;
        in_buffer(COL+1,1) = in_buffer(COL,0);
        in_buffer[0] = threshold_bit;
        out_buffer((COL+1)*8 + 7,8) = out_buffer((COL)*8 + 7,0);
        out_bufferW((COL+1)*8 + 7,8) = out_bufferW((COL)*8 + 7,0);
        connected_cW = conn_comp_1st_pass_white(in_buffer, &out_bufferW, un_classW, COL, ROW, column_value, row_value, &labelNoW);
        //printf(" %d,   ", (int)labelNoW);
        
        out_bufferW(7,0) = connected_cW;
        //if(j = 42024){
        //  //printf("%d ,  ", (int)connected_cW);
        //  check = out_bufferW;
        //}
        connected_c = conn_comp_1st_pass_black(in_buffer, &out_buffer, un_class, COL, ROW, column_value, row_value, label, out_bufferW, &labelNoB);
        out_buffer(7,0) = connected_c;
        size[connected_c] +=1;
        column_value += 1;
        if (column_value >= COL) {
          row_value+=1;
          column_value = 0;
        }

      }
      for(int i = 1; i >= 0; i--){
        #pragma unroll
        pixel chunk = input_lo((i << 3) + 7, (i << 3));
        bit threshold_bit = threshold_image(chunk, threshold_value);
        // Connected components
        pixel connected_c2;
        pixel connected_cW2;
        in_buffer2(COL+1,1) = in_buffer2(COL,0);
        in_buffer2[0] = threshold_bit;
        out_buffer2((COL+1)*8 + 7,8) = out_buffer2((COL)*8 + 7,0);
        out_bufferW2((COL+1)*8 + 7,8) = out_bufferW2((COL)*8 + 7,0);
        connected_cW2 = conn_comp_1st_pass_white(in_buffer2, &out_bufferW2, un_classW2, COL, ROW, column_value2, row_value2, &labelNoW2);
        out_bufferW2(7,0) = connected_cW2;
        connected_c2 = conn_comp_1st_pass_black(in_buffer2, &out_buffer2, un_class2, COL, ROW, column_value2, row_value2, label2, out_bufferW2, &labelNoB2);
        out_buffer2(7,0) = connected_c2;
        size2[connected_c2] +=1;
        column_value2 += 1;
        if (column_value2 >= COL) {
          row_value2+=1;
          column_value2 = 0;
        }
      }
    }
    //for(int i = 0; i<410; i++){
    //  //printf("bottum middle class %d\n", (int)mid_bW[i]);
    //  if(out_bufferW(i*8+7, i*8) !=0 ){//
    //    printf("upper middle class %d\n//",(int)out_bufferW(i*8+7, i*8));
    //  }//
    //  
    //  //if(mid_bW[i] == 155){
    //  //  printf("----------------------------------------");
    //  //}
    //}
    mergeW( out_bufferW, mid_bW, mid_bval, in_buffer, un_classW, un_classW2);
    //mergeB( out_buffer, out_buffer2, mid_bval, in_buffer, size, size2 );
    //two input buffers
    //four output buffers
    //two un_class
    //two un_classW
    //two label
    //two size
    //label number input to connected components
    //store middle labeled lines
    //might have to change cc, set up middle row like top row (y and x)
    
    //divide black dot, 1/2 black dot un_class = 0
    //add a check in douple for loop to exclude 0
    //add size[2nd half]+= 1st halft
    //split white dot
    //2nd half, start with higher noW, 1-100 for 1st laft and 100 to 256 for 2nd
    //combine white dot, un_class[] = 2nd half number
    int black_dots = 0;
    for (int k= 255; k>=0; k--){
      int add = 1;
      int add2 = 1;
      int add_size = 1;
      int add_size2 = 1;
      int name = un_class[k];
      int name2 = un_class2[k];
      for (int m= 0; m<255; m++){
        if(name == un_class[m] && m<k){
          add = 0;
          if(add_size){
            size[m] += size[k];
            add_size = 0;
          }
        }
        if(name2 == un_class2[m] && m<k){
          add2 = 0;
          if(add_size2){
            size2[m] += size2[k];
            add_size2 = 0;
          }
        }
      }
      if(add){
        black_dots++;
        //printf("black dot size %d\n", size[k]);
        bool cont = size[k]>6 && size[k]<100;
        int ind = un_classW[label[name]];
        pixel next_W = un_classW[ind];
        if(next_W != 0 && cont){
          //printf("size is %d\n",size[k]);
          dice_value[next_W] +=1;
        }
      }
      if(add2){
        bool cont = size2[k]>6 && size2[k]<100;
        pixel next_W = un_classW2[label2[name]];
        if(next_W != 0 && cont){
          //printf("size is %d\n",size2[k]);
          dice_value[next_W] +=1;
        }
      }
    }
    
    //printf("black dots %d", black_dots);
    int count = 0;
    //int save = 0;
    for (int l = 0; l<256; l++){
      if(dice_value[l]>0){
        //if(dice_value[l] == 1){
        //  //printf("white class is %d\n", l);
        //  save +=1;
        //}
        //else{
        strm_out.write(dice_value[l]);
        //}
        
        //printf("%d\n",dice_value[l]);
        count++;
      }
    }
    //for (int m = 0; m<save; m++){
    //  strm_out.write(1);
    //}
  }
}