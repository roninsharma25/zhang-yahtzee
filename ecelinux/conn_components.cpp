#include <cmath>
#include <stdio.h>
//#include <iostream>
#include "conn_components.h"
#include <cassert> 

// https://www.codeproject.com/Articles/825200/An-Implementation-Of-The-Connected-Component-Label

//using namespace std;

bit3_t labelNo = 1;
bit3_t conn_comp_1st_pass( buf_bit in_buffer, buf_3 out_buffer, bit3_t un_class[20], int width, int height, int x, int y) {
  bit3_t output;
  bit in = in_buffer[0];
  if(in==1){
    return 0;
  }
  bit A = y == 0? 1: in_buffer[width];
  bit B = x== 0? 1: in_buffer[1];
  bit C = ((x==0)||(y==0))?1: in_buffer[width+1];//8
  bit D = ((x==width-1)||(y==0))?1: in_buffer[width-1];
  if(A && B && C && D){//6
    //output_image[index] = labelNo;
    output= labelNo;
    un_class[labelNo] = labelNo;
    //if(labelNo == 4){
    //  printf("a is %d", (int)A);
    //  printf("b is %d", (int)B);
    //  printf("c is %d", (int)C);
    //  printf("d is %d", (int)D);
    //  printf("x is %d", x);
    //}
    //printf("new class\n");
    //printf("%d", (int)labelNo);
    labelNo++;
    assert (labelNo<8);
  }
  else{
    //bit6_t labelA = out_buffer(width*6+5,width*6);
    //bit6_t labelB = out_buffer(11,6);
    //bit6_t labelC = out_buffer((width+1)*6+5,(width+1)*6);
    //bit6_t labelD = out_buffer((width-1)*6+5,(width-1)*6);
    bit6_t labelA = out_buffer(width*3+2,width*3);
    bit6_t labelB = out_buffer(5,3);
    bit6_t labelC = out_buffer((width+1)*3+2,(width+1)*3);
    bit6_t labelD = out_buffer((width-1)*3+2,(width-1)*3);
    if((A==0)&&((labelA<=labelB)||B == 1) && ((labelA<=labelC)||C==1)&&((labelA<=labelD)||D==1)){
            //printf("above\n");
            //output_image[index] = labelA;
      output = labelA;
      if(B==0){
        un_class[labelB] = labelA;
      }
      if(C==0){
        un_class[labelC] = labelA;
      }
      if(D==0){
         un_class[labelD] = labelA;
      }
    }
    else if((B==0)&&((labelB<=labelA)||A == 1) && ((labelB<=labelC)||C==1)&&((labelB<=labelD)||D==1)){
      //printf("left\n");
      //output_image[index] = labelB;
      output = labelB;
      if(A==0){
        un_class[labelA] = labelB;
      }
      if(C==0){
        un_class[labelC] = labelB;
      }
      if(D==0){
         un_class[labelD] = labelB;
      }
    }
    else if((C==0)&&((labelC<=labelA)||A == 1) && ((labelC<=labelB)||B==1)&&((labelC<=labelD)||D==1)){
      //printf("diag\n");
      //output_image[index] = labelC;
      output = labelC;
      if(A==0){
        un_class[labelA] = labelC;
      }
      if(B==0){
        un_class[labelB] = labelC;
      }
      if(D==0){
         un_class[labelD] = labelC;
      }
    }
    else{
      //printf("diagonal2\n");
      //output_image[index] = labelD;
      output = labelD;
      if(A==0){
        un_class[labelA] = labelD;
      }
      if(B==0){
        un_class[labelB] = labelD;
      }
      if(C==0){
         un_class[labelC] = labelD;
      }
    }
  }
  return output;
        /*
        bit3_t output;
        bit in = in_buffer[0];
        if(in==0){
          return 0;
        }
        printf("not 0\n");
        bit A = y == 0? 0: in_buffer[width];
        bit B = x== 0? 0: in_buffer[1];
        bit C = ((x==0)||(y==0))?0: in_buffer[width+1];//8
        bit D = ((x==width-1)||(y==0))?0: in_buffer[width-1];
        if(A == 0 && B == 0 && C== 0 && D==0){//6
          //output_image[index] = labelNo;
          output= labelNo;
          un_class[labelNo] = labelNo;
          printf("new class\n");
          labelNo++;

          assert (labelNo<8);
        }
        else{
          //bit6_t labelA = out_buffer(width*6+5,width*6);
          //bit6_t labelB = out_buffer(11,6);
          //bit6_t labelC = out_buffer((width+1)*6+5,(width+1)*6);
          //bit6_t labelD = out_buffer((width-1)*6+5,(width-1)*6);
          bit6_t labelA = out_buffer(width*3+2,width*3);
          bit6_t labelB = out_buffer(5,3);
          bit6_t labelC = out_buffer((width+1)*3+2,(width+1)*3);
          bit6_t labelD = out_buffer((width-1)*3+2,(width-1)*3);
          if((A==1)&&((labelA<=labelB)||B == 0) && ((labelA<=labelC)||C==0)&&((labelA<=labelD)||D==0)){
            //printf("above\n");
            //output_image[index] = labelA;
            output = labelA;
            if(B){
              un_class[labelB] = labelA;
            }
            if(C){
              un_class[labelC] = labelA;
            }
            if(D){
               un_class[labelD] = labelA;
            }
          }
          else if((B==1)&&((labelB<=labelA)||A == 0) && ((labelB<=labelC)||C==0)&&((labelB<=labelD)||D==0)){
            //printf("left\n");
            //output_image[index] = labelB;
            output = labelB;
            if(A){
              un_class[labelA] = labelB;
            }
            if(C){
              un_class[labelC] = labelB;
            }
            if(D){
               un_class[labelD] = labelB;
            }
          }
          else if((C==1)&&((labelC<=labelA)||A == 0) && ((labelC<=labelB)||B==0)&&((labelC<=labelD)||D==0)){
            //printf("diag\n");
            //output_image[index] = labelC;
            output = labelC;
            if(A){
              un_class[labelA] = labelC;
            }
            if(B){
              un_class[labelB] = labelC;
            }
            if(D){
               un_class[labelD] = labelC;
            }
          }
          else{
            //printf("diagonal2\n");
            //output_image[index] = labelD;
            output = labelD;
            if(A){
              un_class[labelA] = labelD;
            }
            if(B){
              un_class[labelB] = labelD;
            }
            if(C){
               un_class[labelC] = labelD;
            }
          }
        }
        return output;
*/
/*
  for (int y = 0; y < height; y++)
  {
    for (int x = 0; x < width; x++)
    {
        index = x + width*y;
        bit input = input_image[index];
        buffer(ROW+1,1) = buffer(ROW,0);
        buffer(0) = input;
        out_buffer(ROW+1,1) = out_buffer(ROW,0);
        if (input == 0){
            out_buffer(0) = 0;
            //output_image[index] = 0;
            continue;
        }
        //int above = x+ (width)*(y-1);//ROW+1
        //int left = x-1 + width*y;//1
        //int diag = x-1 + (width)*(y-1);//ROW+2
        //int diag2 = x+1 + (width)*(y-1);//ROW
        int A = y == 0? 0: buffer(ROW);
        int B = x== 0? 0: buffer(1);
        int C = ((x==0)||(y==0))?0: buffer(ROW+1);//8
        int D = ((x==width-1)||(y==0))?0: buffer(ROW-1);
        if(A == 0 && B == 0 && C== 0 && D==0){//6
          //output_image[index] = labelNo;
          out_buffer(0) = labelNo;
          un_class[labelNo] = labelNo;
          //printf("new class");
          labelNo++;
          //assert (labelNo<20);
        }
        else{
          int labelA = output_image[above];
          int labelB = output_image[left];
          int labelC = output_image[diag];
          int labelD = output_image[diag2];
          if((A==1)&&((labelA<=labelB)||B == 0) && ((labelA<=labelC)||C==0)&&((labelA<=labelD)||D==0)){
            printf("above\n");
            //output_image[index] = labelA;
            out_buffer(0) = labelA;
            if(B){
              un_class[labelB] = labelA;
            }
            if(C){
              un_class[labelC] = labelA;
            }
            if(D){
               un_class[labelD] = labelA;
            }
          }
          else if((B==1)&&((labelB<=labelA)||A == 0) && ((labelB<=labelC)||C==0)&&((labelB<=labelD)||D==0)){
            printf("left\n");
            //output_image[index] = labelB;
            out_buffer(0) = labelB;
            if(A){
              un_class[labelA] = labelB;
            }
            if(C){
              un_class[labelC] = labelB;
            }
            if(D){
               un_class[labelD] = labelB;
            }
          }
          else if((C==1)&&((labelC<=labelA)||A == 0) && ((labelC<=labelB)||B==0)&&((labelC<=labelD)||D==0)){
            printf("diag\n");
            //output_image[index] = labelC;
            out_buffer(0) = labelC;
            if(A){
              un_class[labelA] = labelC;
            }
            if(B){
              un_class[labelB] = labelC;
            }
            if(D){
               un_class[labelD] = labelC;
            }
          }
          else{
            printf("diagonal2\n");
            //output_image[index] = labelD;
            out_buffer(0) = labelD;
            if(A){
              un_class[labelA] = labelD;
            }
            if(B){
              un_class[labelB] = labelD;
            }
            if(C){
               un_class[labelC] = labelD;
            }
          }
        }
        output_image[index] = out_buffer(0);
    }
  }
  */
}
void conn_comp_2nd_pass(int input_image[NUM_PIXELS], int output_image[NUM_PIXELS], int* un_class, int width, int height ){
  int size_class[20];
  for (int y = 0; y < height; y++)
  {
    for (int x = 0; x < width; x++)
    {
        int index = x + width*y;
        int label = input_image[index];
        //printf("label %d \n", label);
        if(label!=0){
            int correct_label = un_class[label];
            //sprintf("correct_label %d \n", correct_label);
            size_class[correct_label] += 1;
            if(correct_label!=label){
                output_image[index] = correct_label;
            }
        }
    }
  }
}


