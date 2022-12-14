//=========================================================================
// testbench.cpp
//=========================================================================
// @brief: testbench for dice digit recognition application

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

#include "digitrec.h"
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
  dr = opendir("data");
      
  // Output file that saves the test bench results
  std::ofstream outfile;
  outfile.open("output/out_conn_comp.txt");
  
  // Read input file for the testing set
  std::string line;
  //std::ifstream myfile ("data/output_formatted.txt");
  std::ifstream myfile ("data/5d_1_4_3_5_3_410.txt");
  
  // HLS streams for communicating with the cordic block
  hls::stream<bit32_t> in_stream;
  hls::stream<pixel> out_stream;

  std::string test_str = "5d_1_4_3_5_3.txt";

  std::string subset2 = test_str.substr(0,1);
  int num_digits = atoi(subset2.c_str());
  int dice_values[num_digits];

  int current_index = 3;
  for (int i = 0; i < num_digits; i++) {
    dice_values[i] = atoi(test_str.substr(current_index, 1).c_str());
    current_index += 2;
  }

  for (int i = 0; i < num_digits; i++) {
    printf("INPUT NUM: %d \n", dice_values[i]);
  }



  // Number of test instances

 // const int N = 42025;
  const int N = 42025;

  
  // Arrays to store test data and expected results
  bit32_t inputs[N];

  // Timer
  Timer timer("digitrec FPGA");
  
  int nbytes;
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
    
    for (int i = 0; i < N; ++i ) {
      // Read input from array and split into two 32-bit words
      bit32_t input_lo = inputs[i].range(31,0);
      // Write words to the device
      in_stream.write( input_lo );
    }
    // //--------------------------------------------------------------------
    // // Execute the digitrec sim and receive data
    // //--------------------------------------------------------------------



    dut( in_stream, out_stream);

    pixel threshold_value = out_stream.read();
    printf("threshold value: %d \n", threshold_value.to_int());


    // Analyze the outputs
    int count = 0;
    int num_correct = 0;
    int num_tests = 0;

    for (int i = 0; i < num_digits; ++i ) {
      int dice_num = out_stream.read();
      printf("Dice Classification: %d \n", dice_num);

      if (dice_num == dice_values[i]) {
        num_correct++;
      }
      num_tests++;
    }

    std::cout << "Overall Accuracy Rate = " << std::setprecision(3)
              << ( (double)num_correct / num_tests ) * 100
              << "% \n";

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