//=========================================================================
// testbench.cpp
//=========================================================================
// @brief: testbench for k-nearest-neighbor digit recongnition application

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

#include "digitrec.h"
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

//------------------------------------------------------------------------
// Digitrec testbench
//------------------------------------------------------------------------
int main() 
{
  // Output file that saves the test bench results
  std::ofstream outfile;
  outfile.open("output/out_conn_comp.txt");
  
  // Read input file for the testing set
  std::string line;
  //std::ifstream myfile ("data/output_formatted.txt");
  //std::ifstream myfile ("output/5d14353.txt");
  std::ifstream myfile ("5d26361.txt");
  
  // HLS streams for communicating with the cordic block
  hls::stream<bit32_t> in_stream;
  hls::stream<pixel> out_stream;

  // Number of test instances
  const int N = 42025;
  
  // Arrays to store test data and expected results
  bit32_t inputs[N];

  // Timer
  Timer timer("digitrec FPGA");
  
  int nbytes;
  int error = 0;
  int num_test_insts = 0;
  int rows = 410;
  int cols = 410;
  bit32_t threshold_value;


  if ( myfile.is_open() ) {
    assert( std::getline( myfile, line) );
    // std::string row_string = line.substr(0, line.find(","));
    // std::string col_string = line.substr(line.find(", "), line.length());
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
      bit32_t input_lo = inputs[i].range(31,0);
      // Write words to the device
      in_stream.write( input_lo );
    }

    //--------------------------------------------------------------------
    // Execute the digitrec sim and receive data
    //--------------------------------------------------------------------
    dut( in_stream, out_stream, rows, cols, 1);

    pixel threshold_value = out_stream.read();

    printf("threshold value: %d \n", threshold_value.to_int());

    for (int i = 0; i < N; ++i ) {
      // Read input from array and split into two 32-bit words
      bit32_t input_lo = inputs[i].range(31,0);
      // Write words to the device
      in_stream.write( input_lo );
    }

    dut( in_stream, out_stream, rows, cols, 0);

    int count = 0;
    for (int i = 0; i < (168100); ++i ) {
      // Write words to the device
      pixel conn_comp = out_stream.read();

      outfile << conn_comp.to_int() << "\n";
      count++;
      //printf("iteration: %d \n", count);

      // if (count >= 410) {
      //   outfile << "\n";
      //   count = 0;
      // }
    }


    // for (int i = 0; i < N; ++i ) {
    //   // Read input from array and split into two 32-bit words
    //   bit32_t input_lo = inputs[i].range(31,0);
    //   // Write words to the device
    //   in_stream.write( input_lo );
    // }

    // for (int i = 0; i < N; ++i ) {
    //   // Call design under test (DUT)
    //   dut( in_stream, out_stream );
    //   // Read result
    //   for(int i = 0; i < 4; i++){
    //     pixel threshold_pixel = out_stream.read();
    //     printf("%d", threshold_pixel.to_int());
    //   }
    //   num_test_insts++;  
    // }   

    timer.stop();
    
    // Close input file for the testing set
    myfile.close();
    
  }
  else
      std::cout << "Unable to open file for the testing set!" << std::endl; 
  
  // Close output file
  outfile.close();

  return 0;
}
