//=========================================================================
// testbench.cpp
//=========================================================================
// @brief: testbench for dice digit recongnition application

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

#include "digitrec.h"
#include "timer.h"

#include <dirent.h>

int num_correct = 0;
int num_tests = 0;

using namespace std;

//------------------------------------------------------------------------
// Helper function for hex to int conversion
//------------------------------------------------------------------------
int64_t hexstring_to_int64 (string h) {
  int64_t x = 0;
  for (int i = 0; i < h.length(); ++i) {
    char c = h[i];
    int k = (c > '9') ? toupper(c)-'A'+10 : c - '0';
    x = x*16 + k;
  }
  return x;
}

void run_algorithm(string file_name) {
  // Output file that saves the test bench results
  ofstream outfile;
  outfile.open("output/out_conn_comp.txt");
  cout << file_name << "\n";
  
  // Read input file for the testing set
  string line;
  string data_path = "data/";
  string input_file_path = data_path + file_name;
  ifstream myfile (input_file_path.c_str());
  
  // HLS streams for communicating with the cordic block
  hls::stream<bit32_t> in_stream;
  hls::stream<pixel> out_stream;

  string subset2 = file_name.substr(0,1);
  int num_digits = atoi(subset2.c_str());
  int dice_values[num_digits];
  int dice_values_acc[7] = {0, 0, 0, 0, 0, 0};

  int current_index = 3;
  for (int i = 0; i < num_digits; i++) {
    int num = atoi(file_name.substr(current_index, 1).c_str());
    dice_values[i] = num;
    dice_values_acc[num - 1] += 1;
    current_index += 2;
  }

  for (int i = 0; i < num_digits; i++) {
    printf("INPUT NUM: %d \n", dice_values[i]);
  }

  // Number of test instances
  const int N = (ROW*COL)/4; 
  
  // Arrays to store test data and expected results
  bit32_t inputs[N];

  // Timer
  Timer timer("digitrec FPGA");
  
  int nbytes;
  int error = 0;
  int num_test_insts = 0;
  bit32_t threshold_value;


  if ( myfile.is_open() ) {
    assert( getline( myfile, line) );
    for (int i = 0; i < N; ++i) {
      assert( getline( myfile, line) );
      // Read handwritten digit input
      string hex_digit = line.substr(2);
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
    }

    //--------------------------------------------------------------------
    // Execute the digitrec sim and receive data
    //--------------------------------------------------------------------

    for (int i = 0; i < N; ++i ) {
      // Read input from array and split into two 32-bit words
      bit32_t input_lo = inputs[i].range(31,0);
      // Write words to the device
      in_stream.write( input_lo );
    }

    dut( in_stream, out_stream );

    // Analyze the outputs
    for (int i = 0; i < num_digits; ++i ) {
      int dice_num = out_stream.read();
      printf("Dice Classification: %d \n", dice_num);
      dice_values_acc[dice_num - 1] -= 1;
      num_tests++;
    }

    int num_wrong = 0; 
    for (int i = 0; i < 6; i++){
      printf("Dice %d: %d \n", i + 1, dice_values_acc[i]);
      if (dice_values_acc[i] > 0){
        num_wrong += dice_values_acc[i];
      } 
    }
    num_correct += num_digits - num_wrong;
    timer.stop();
    
    // Close input file for the testing set
    myfile.close();
    
  }
  else
      cout << "Unable to open file for the testing set!" << endl; 
  
  // Close output file
  outfile.close();
}

//------------------------------------------------------------------------
// Digitrec testbench
//------------------------------------------------------------------------
int main()
{
  DIR *dr;
  struct dirent *en;
  dr = opendir("data");
  if (dr) {
    // Loop through all files in the directory
    int count2 = 0;
    while ((en = readdir(dr)) != NULL) {
      string file_name = en->d_name;
      if (file_name.find("txt") != string::npos) {
        run_algorithm(file_name);
        count2++;
        //if (count2 >= 10) break;
      }
    }
    closedir(dr);
  }

  cout << "Overall Accuracy Rate = " << setprecision(3)
              << ( (double)num_correct / num_tests ) * 100
              << "% \n";

  return 0;
}
