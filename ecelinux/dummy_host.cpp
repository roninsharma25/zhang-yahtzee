//=========================================================================
// testbench.cpp
//=========================================================================
// @brief: testbench for dummy host

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

#include "typedefs.h"
#include "timer.h"

#include <dirent.h>

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

//------------------------------------------------------------------------
// Digitrec testbench
//------------------------------------------------------------------------
int main() 
{
 
  Timer timer("dummy");
  int N = 42025;
  int nbytes;
  int error = 0;
  int num_test_insts = 0;
  int realsum = 0;
  int num_digits = 7;
  int num_correct = 0;
  int num_tests = 0;

  for (int i = 0; i < N; ++i ) {
    realsum = realsum + i;
  }

  timer.start();

  //--------------------------------------------------------------------
  // Add your code here to communicate with the hardware module
  //--------------------------------------------------------------------


  for (int i = 0; i < N; ++i ) {
    // Read input from array and split into two 32-bit words
    bit32_t input_lo = i;
    // Write words to the device
    nbytes = write (fdw, (void*)&input_lo, sizeof(input_lo));
    assert(nbytes == sizeof(input_lo));
  }

  for (int i = 0; i < num_digits; ++i ) {
    bit32_t dice_num;
    nbytes = read (fdr, (void*)&dice_num, sizeof(dice_num));
    assert (nbytes == sizeof(dice_num));
    if (dice_num == realsum) {
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

  // Report overall error out of all testing instances
  std::cout << "Overall Accuracy Rate = " << std::setprecision(3)
            << ( (double)num_correct / num_tests ) * 100
            << "% \n";
 

  return 0;
}
