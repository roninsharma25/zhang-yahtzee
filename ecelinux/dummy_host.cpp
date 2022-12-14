//=========================================================================
// testbench.cpp
//=========================================================================
// @brief: testbench for dummy host

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

#include "dummy.h"
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
 
  DIR *dr;
  struct dirent *en;
  dr = opendir("small_data");
  if (dr) {
    // Loop through all files in the directory
    while ((en = readdir(dr)) != NULL) {
      std::string s = en->d_name;
      if (s.find("txt") != std::string::npos) {
        std::cout << s << "\n";
      }
    }
    closedir(dr);
  }

  std::string line;

  std::ifstream myfile ("small_data/5d_3_2_2_4_3.txt");

  std::string test_str = "5d_3_2_2_4_3.txt";

  std::string subset2 = test_str.substr(0,1);
  int num_digits = atoi(subset2.c_str());
  int dice_values[num_digits];

  int current_index = 3;
  for (int i = 0; i < num_digits; i++) {
    dice_values[i] = atoi(test_str.substr(current_index, 1).c_str());
    current_index += 2;
  }

  // HLS streams for communicating with the cordic block
  hls::stream<bit32_t> in_stream;
  hls::stream<bit32_t> out_stream;

  const int N = 11025;

  // Arrays to store test data and expected results
  bit32_t inputs[N];

  // Timer
  Timer timer("digitrec FPGA");

  if ( myfile.is_open() ) {
    assert( std::getline( myfile, line) );
    for (int i = 0; i < N; ++i) {
      assert( std::getline( myfile, line) );
      // Read handwritten digit input
      std::string hex_digit = line.substr(2);
      bit32_t input_digit = hexstring_to_int64 (hex_digit);
      // Store the digits into arrays
      inputs[i] = input_digit;
    }

    timer.start();

    //--------------------------------------------------------------------
    // Send data digitrec
    //--------------------------------------------------------------------

    for (int i = 0; i < N; ++i ) {
      // Read input from array and split into two 32-bit words
      bit32_t input_lo = inputs[i];
      // Write words to the device
      in_stream.write( input_lo );
    }

    dut( in_stream, out_stream );

    for (int i = 0; i < NUMDIE; i++){
      int dice_num = out_stream.read();
    }

    timer.stop();
    
  

    // Close input file for the testing set
    myfile.close();
  
  }
  else
    std::cout << "Unable to open file for the testing set!" << std::endl; 


  return 0;
}
