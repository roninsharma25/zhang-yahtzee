from PIL import Image
import numpy as np
import sys


def load_data_from_file(file_name):
    # Convert image to grayscale
    img = Image.open( file_name ).convert('L')
    img.load()
    data = np.asarray( img, dtype = 'int32' )

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


def main():
    # Retrieve optional arguments
    num_arguments = len(sys.argv)
    #input_file_name = sys.argv[1] if (num_arguments >= 2) else '../assets/5d_1_4_3_5_3.jpg'
    #output_file_name = sys.argv[2] if (num_arguments >= 3) else '../ecelinux/output_formatted.txt'
    input_file_name = sys.argv[1] if (num_arguments >= 2) else '../assets/7d_4_6_2_3_1_5_5.jpg'
    output_file_name = sys.argv[2] if (num_arguments >= 3) else '../ecelinux/7d4623155.txt'

    # Load input data
    data = load_data_from_file(input_file_name)

    # Obtain the initial dimensions
    rows = data.shape[0]
    columns = data.shape[1]

    # Convert 2D array to 1D
    data = data.flatten()

    # Create an array of hex values
    hex_array = create_hex_array(data)

    # Write the output data to a file
    hex_array = ['{}, {}'.format(rows, columns)] + hex_array
    write_data_to_file(output_file_name, hex_array)


if __name__ == '__main__':
    main()
