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

  // Output file that saves the test bench results
  ofstream outfile;
  outfile.open("output/out_conn_comp.txt");
  
  // Read input file for the testing set
  string line;
  string data_path = "data/";
  string input_file_path = data_path + file_name;
  ifstream myfile (input_file_path.c_str());

  string subset2 = file_name.substr(0,1);
  int num_digits = atoi(subset2.c_str());
  int dice_values[num_digits];
  int dice_values_acc[6] = {0, 0, 0, 0, 0, 0};

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
    
    //--------------------------------------------------------------------
    // Read data from the input file into two arrays
    //--------------------------------------------------------------------
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
    // Add your code here to communicate with the hardware module
    //--------------------------------------------------------------------


    //--------------------------------------------------------------------
    // Send data digitrec
    //--------------------------------------------------------------------
    for (int i = 0; i < sizeof(inputs); ++i ) {
      // Read input from array and split into two 32-bit words
      bit32_t input_lo = inputs[i].range(31,0);
      // Write words to the device
      //in_stream.write( input_lo );
      nbytes = write (fdw, (void*)&input_lo, sizeof(input_lo));
      assert(nbytes == sizeof(input_lo));

    }

    //--------------------------------------------------------------------
    // Execute the digitrec sim and receive data
    //--------------------------------------------------------------------
    //dut( in_stream, out_stream, 0, 0, 1);

    //pixel threshold_value = out_stream.read();

    pixel threshold_value;
    nbytes = read (fdr, (void*)&threshold_value, sizeof(threshold_value));
    assert (nbytes == sizeof(threshold_value));

    printf("threshold value: %d \n", threshold_value.to_int());

    for (int i = 0; i < N; ++i ) {
      // Read input from array and split into two 32-bit words
      bit32_t input_lo = inputs[i].range(31,0);
      // Write words to the device
      //in_stream.write( input_lo );
      nbytes = write (fdw, (void*)&input_lo, sizeof(input_lo));
      assert(nbytes == sizeof(input_lo));
    }

    //dut( in_stream, out_stream, rows, cols, 0);

    // Analyze the outputs
    for (int i = 0; i < num_digits; ++i ) {
      //int dice_num = out_stream.read();
      int dice_num;
      
      nbytes = read (fdr, (void*)&dice_num, sizeof(dice_num));
      assert (nbytes == sizeof(dice_num));

      dice_values_acc[dice_num - 1] -= 1;
      num_tests++;
    }

    int num_wrong = 0; 
    for (int i = 0; i < 6; i++){
      
      if (dice_values_acc[i] > 0){
        num_wrong += dice_values_acc[i];
      } 
    }
    num_correct += num_digits - num_wrong;

    timer.stop();
    
    printf("Dice Classification: %d \n", dice_num);
    printf("Dice %d: %d \n", i, dice_values_acc[i]);

    // Close input file for the testing set
    myfile.close();
  }
  else
      std::cout << "Unable to open file for the testing set!" << std::endl; 
  // Close output file
  outfile.close();

  return 0;
}
