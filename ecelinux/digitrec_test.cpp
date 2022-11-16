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

  // std::ifstream myfile ("output/5d14353.txt");

  // Test with a small input image
  std::ifstream myfile ("output/small_input.txt");
  
  // HLS streams for communicating with the cordic block
  hls::stream<bit32_t> in_stream;
  hls::stream<pixel> out_stream;

  // Number of test instances
  const int N = 4; //42025;
  
  // Arrays to store test data and expected results
  bit32_t inputs[N];

  // Timer
  Timer timer("digitrec FPGA");
  
  int nbytes;
  int error = 0;
  int num_test_insts = 0;
  int rows = 4; //410;
  int cols = 4; //410;
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


    // for (int i = 0; i < NUM_PIXELS; ++i ) {
    //   // Write words to the device
    //   pixel conn_comp = out_stream.read();

    //   outfile << conn_comp.to_int() << "\n";
    //   printf("conn comp: %d \n", conn_comp.to_int());
    // }

    for (int i = 0; i < ROW_COUNT; ++i ) {
      for (int j = 0; j < ROW_COUNT; ++j) {
        // Write words to the device
        pixel conn_comp = out_stream.read();

        outfile << conn_comp.to_int() << " ";
        // printf("conn comp: %d \n", conn_comp.to_int());
      }
      outfile << "\n";
    }

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
