from PIL import Image
import numpy as np
import sys
import copy

MAX = 410
MAX_COL = 210
MAX_ROW = 210 

def load_data_from_file(file_name):
    # Convert image to grayscale
    img = Image.open( file_name ).convert('L')
    img.load()
    data = np.asarray( img, dtype = 'uint8' ) #was int32

    return data


def create_hex_array(data):
    hex_array = []
    temp = []
    for pixel_value in data:
        hex_value = hex(pixel_value)[2:]
        hex_value = '0' + hex_value if len(hex_value) == 1 else hex_value

        if (len(temp) < 4):
            temp.append(hex_value)
        else:
            hex_array.append('0x' + ''.join(temp))
            temp = [ hex_value ]

    hex_array.append('0x' + ''.join(temp))

    return hex_array


def write_data_to_file(file_name, data):
    with open(file_name, 'w') as f:
        for row in data:
            f.write(row + '\n')

def data_analysis(data): 
    temp = [] 
    row_data = [] 
    column_data = []
    number = 0
    row = 0
    row_num = 0 
    for pixel_value in data: 
        column = 0
        row_largest = 0
        for number in pixel_value:   
            if (row_largest < number): row_largest = number
            if (row == 0): column_data.append(number)
            elif (column_data[column] < number): column_data[column] = number
            column += 1
        row_data.append(row_largest)
        row += 1
        
    row_data_sort = copy.deepcopy(row_data)
    row_data_sort.sort()
    #print(row_data)
    #print(row_data_sort)
    row = 0
    for pixel_value in data: 
        if (row_data[row] > row_data_sort[MAX - MAX_ROW]): 
            row_num += 1
            temp.append(pixel_value)
        row += 1 

    temp = column_remove(temp, column_data, row_num)
    return temp

def column_remove(data, column_data, row_num): 
    temp = [] 
    row_data = [] 
    row = 0
    column_data_sort = copy.deepcopy(column_data)
    column_data_sort.sort()
    for pixel_value in data: 
        column = 0
        col_num = 0
        for number in pixel_value: 
            if (column_data[column] > column_data_sort[MAX - MAX_COL]): 
                row_data.append(number)
                col_num += 1
            column += 1 
        # column padding
        for x in range (col_num, MAX_COL):
            row_data.append(0)
        temp.append(row_data)
        row_data = []   
    #row padding
    row = [] 
    for x in range(0, MAX_COL): 
        row.append(0)
    for x in range(row_num, MAX_ROW):
        temp.append(row)
    return np.asarray(temp, dtype = 'uint8') #may need to change to uint32? 

def create_image(image_array, file_name):
    image = Image.fromarray(image_array, mode="L") 
    image.save(file_name)

def main():
    # Retrieve optional arguments
    num_arguments = len(sys.argv)
    input_file_name = sys.argv[1] if (num_arguments >= 2) else '../assets/5d_1_4_3_5_3.jpg'
    output_file_name = sys.argv[2] if (num_arguments >= 3) else 'output_formatted.txt'

    # Load input data
    data = load_data_from_file(input_file_name)
    data_check = data_analysis(data)
    
    #compare the photos to each other (comment out later)
    c_array = np.asarray(data_check)
    c_array_1 = np.asarray(data)
    create_image(c_array, 'After.png')
    create_image(c_array_1, 'Before.png')
  

    # Obtain the initial dimensions
    rows = data_check.shape[0]
    columns = data_check.shape[1]
    # Convert 2D array to 1D
    data_check = data_check.flatten()
    
    # Create an array of hex values
    hex_array = create_hex_array(data_check)

    # Write the output data to a file
    hex_array = ['{}, {}'.format(rows, columns)] + hex_array
    write_data_to_file(output_file_name, hex_array)


if __name__ == '__main__':
    main()
