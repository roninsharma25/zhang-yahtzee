#include <cmath>
#include <stdio.h>
//#include <iostream>
#include "conn_comp_forloop.h"

// https://www.codeproject.com/Articles/825200/An-Implementation-Of-The-Connected-Component-Label

//using namespace std;


void conn_comp(int input_image[NUM_PIXELS], int* output_image, int width, int height) {
  int labelNo = 1;
  int index   = 0;
  int un_class[20];
  for (int y = 0; y < height; y++)
  {
    for (int x = 0; x < width; x++)
    {
        index = x + width*y;
        if (input_image[index] == 0){
            output_image[index] = 0;
            continue;
        }
        int above = x+ (width)*(y-1);
        int left = x-1 + width*y;
        int diag = x-1 + (width)*(y-1);//12
        int diag2 = x+1 + (width)*(y-1);
        int A = y == 0? 0: input_image[above];
        int B = x== 0? 0: input_image[left];
        int C = ((x==0)||(y==0))?0: input_image[diag];//8
        int D = ((x==width-1)||(y==0))?0: input_image[diag2];
        if(A == 0 && B == 0 && C== 0 && D==0){//6
          output_image[index] = labelNo;
          un_class[labelNo] = labelNo;
          printf("new class");
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
            output_image[index] = labelA;
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
            output_image[index] = labelB;
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
            output_image[index] = labelC;
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
            output_image[index] = labelD;
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
        /*
        else if(A == 1 && B ==1 && C == 1 && D==1){//12
          int labelA = output_image[above];
          int labelB = output_image[left];
          int labelC = output_image[diag];
          int labelD = output_image[diag2];
          if((labelA <= labelB) && (labelA <= labelC) && (labelA <= labelD)){
              output_image[index] = labelA;
              un_class[labelB] = labelA;
              un_class[labelC] = labelA;
              un_class[labelD] = labelA;
              
          }
          else if((labelB <= labelC) && (labelB <= labelC) && (labelB <= labelD)){
              output_image[index] = labelB;
              un_class[labelA] = labelB;
              un_class[labelC] = labelB;
              un_class[labelD] = labelB;
          }
          else if((labelD <= labelC) && (labelD <= labelC) && (labelD <= labelA)){
              output_image[index] = labelD;
              un_class[labelA] = labelD;
              un_class[labelC] = labelD;
              un_class[labelB] = labelD;
          }
          else{
              output_image[index] = labelC;
              un_class[labelA] = labelC;
              un_class[labelB] = labelC;
              un_class[labelD] = labelB;
          }
        }
        else if(A == 1 && B ==1){//5
          int labelA = output_image[above];
          int labelB = output_image[left];
          if(labelA <= labelB){
              output_image[index] = labelA;
              un_class[labelB] = labelA;
          }
          else{
              output_image[index] = labelB;
              un_class[labelA] = labelB;
          }
        }
        else if(A == 1 && C ==1){//5
          int labelA = output_image[above];
          int labelC = output_image[diag];
          if(labelA <= labelC){
              output_image[index] = labelA;
              un_class[labelC] = labelA;
          }
          else{
              output_image[index] = labelC;
              un_class[labelA] = labelC;
          }
        }
        else if(B == 1 && C ==1){//5
          int labelB = output_image[left];
          int labelC = output_image[diag];
          if(labelB <= labelC){
              output_image[index] = labelB;
              un_class[labelC] = labelB;
          }
          else{
              output_image[index] = labelC;
              un_class[labelB] = labelC;
          }
        }
        else if(A == 1){
          int labelA = output_image[above];
          output_image[index] = labelA;
        }
        else if(B == 1){
          int labelB = output_image[left];
          output_image[index] = labelB;
        }
        else if(C == 1){//3
          int labelC = output_image[diag];
          output_image[index] = labelC;
        }
        */
    }
  }
  for (int y = 0; y < height; y++)
  {
    for (int x = 0; x < width; x++)
    {
        index = x + width*y;
        int label = output_image[index];
        //printf("label %d \n", label);
        if(label!=0){
            int correct_label = un_class[label];
            //sprintf("correct_label %d \n", correct_label);
            if(correct_label!=label){
                output_image[index] = correct_label;
            }
        }
    }
  }
}


int main(){
  //FILE *myFile;
    //myFile = fopen("test_image_for_connected.txt", "r");
  int test_array[12];
  //test_array[0] = 1;
  //test_array[1] = 1;
  //test_array[2] = 1;
  //test_array[3] = 0;
  //test_array[4] = 0;
  //test_array[5] = 1;
  //test_array[6] = 1;
  //test_array[7] = 0;
  //test_array[8] = 0;
  test_array[0] = 0;
  test_array[1] = 1;
  test_array[2] = 0;
  test_array[3] = 1;
  test_array[4] = 1;
  test_array[5] = 0;
  test_array[6] = 0;
  test_array[7] = 0;
  test_array[8] = 0;
  test_array[9] = 1;
  test_array[10] = 0;
  test_array[11] = 1;
  int test_output[12];
  conn_comp(test_array, test_output, 3, 4);
  for (int i = 0; i<4; i++){
    for (int j = 0; j<3; j++){
      int index = j+i*3;
      //cout << test_output[index];
      printf("%d", test_output[index]);
    }
    //cout << endl;
    printf("\n");
  }
}


