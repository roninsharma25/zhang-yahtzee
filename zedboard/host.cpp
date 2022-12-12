#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<math.h>
#include<assert.h>

#include<iostream>
#include<fstream>

#include "typedefs.h"
#include "timer.h"

//------------------------------------------------------------------------
// Helper function for hex to int conversion
//------------------------------------------------------------------------
int64_t hexstring_to_int64 (std::string h) {
  int64_t x = 0;
  for (int i = 0; i < h.length(); ++i) {
    char c = h[i];
    int k = (c > '9') ? toupper(c)-'A'+10 : c - '0';
    x = x*16 + k;
  }
  return x;
}

//--------------------------------------
// main function
//--------------------------------------
int main(int argc, char** argv)
{
  // Open channels to the FPGA board.
  // These channels appear as files to the Linux OS
  int fdr = open("/dev/xillybus_read_32", O_RDONLY);
  int fdw = open("/dev/xillybus_write_32", O_WRONLY);

  // Check that the channels are correctly opened
  if ((fdr < 0) || (fdw < 0)) {
    fprintf (stderr, "Failed to open Xillybus device channels\n");
    exit(-1);
  }
  
  // Read input file for the testing set
  std::string line;
  std::ifstream myfile ("data/5d_1_4_3_5_3.txt");
  
  std::string test_str = "5d_1_4_3_5_3.txt";

  std::string subset2 = test_str.substr(0,1);
  int num_digits = atoi(subset2.c_str());
  int dice_values[num_digits];
  int dice_retrieved_values[num_digits];

  int current_index = 3;
  for (int i = 0; i < num_digits; i++) {
    dice_values[i] = atoi(test_str.substr(current_index, 1).c_str());
    current_index += 2;
  }

  for (int i = 0; i < num_digits; i++) {
    printf("INPUT NUM: %d \n", dice_values[i]);
  }


  // Number of test instances
  const int N = (ROW * COL) /4;
  
  // Arrays to store test data and expected results
  bit32_t inputs[N];

  // Timer
  Timer timer("digitrec FPGA");
  
  int nbytes;
  int error = 0;
  int num_test_insts = 0;
  bit32_t interpreted_digit;


  if ( myfile.is_open() ) {
    
    assert( std::getline( myfile, line) );
    for (int i = 0; i < N; ++i) {
      assert( std::getline( myfile, line) );
      std::string hex_digit = line.substr(2);
      bit32_t input_digit = hexstring_to_int64 (hex_digit);
      inputs[i] = input_digit;
    }

    timer.start();

    //--------------------------------------------------------------------
    // Add your code here to communicate with the hardware module
    //--------------------------------------------------------------------


    for (int i = 0; i < N; ++i ) {
      // Read input from array and split into two 32-bit words
      bit32_t input_lo = inputs[i].range(31,0);
      // Write words to the device
      nbytes = write (fdw, (void*)&input_lo, sizeof(input_lo));
      assert(nbytes == sizeof(input_lo));
    }

    int count = 0;
    int num_correct = 0;
    int num_tests = 0;

    for (int i = 0; i < num_digits; ++i ) {
      bit32_t dice_num;
      nbytes = read (fdr, (void*)&dice_num, sizeof(dice_num));
      assert (nbytes == sizeof(dice_num));
      dice_retrieved_values[i] = dice_num;
      if (dice_num == dice_values[i]) {
        num_correct++;
      }
      num_tests++;
    }

    // for (int i = 0; i < sizeof(inputs), i++){
    //   test_digit = inputs[i];

    //   nbytes = write (fdw, (void*)&test_digit, sizeof(test_digit);
    //   assert(nbytes == sizeof(test_digit));

    //   int32_t guess_out;
    //   nbytes = read (fdr, (void*)&guess_out, sizeof(guess_out));
    //   assert (nbytes == sizeof(guess_out));

    //   int guess_i = guess_out;
    //   if(guess_i != expecteds[i]) error++;
    // }  


    timer.stop();
    
    for (int i = 0; i < num_digits; ++i ) {
      printf("Dice Classification: %d \n", dice_retrieved_values[i]);
    }

    // Report overall error out of all testing instances
    std::cout << "Overall Accuracy Rate = " << std::setprecision(3)
              << ( (double)num_correct / num_tests ) * 100
              << "% \n";
 
    // Close input file for the testing set
    myfile.close();
  }
  else
      std::cout << "Unable to open file for the testing set!" << std::endl; 
  
  return 0;
}
