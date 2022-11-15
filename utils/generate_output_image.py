from PIL import Image
import numpy as np
import sys


def load_data_from_file(file_name):
    with open(file_name, 'r') as f:
        input_data = f.readlines()#[0].strip()
    input_data = [ int(x.strip('\n')) for x in input_data]

    return input_data

def process_data(data, flag = True, num_columns = 410): # CHANGE DEFAULT VALUE WHEN NEW DATA IS READY
    output_array = []
    row = []
    for value in data:
        new_value = value
        if (flag):
            new_value = int(value) * 255 
        if (len(row) < num_columns):
            row.append(new_value)
        else:
            output_array.append(row)
            row = [new_value]
        
    output_array.append(row)

    return np.asarray( output_array, dtype = 'uint8' )

def create_image(image_array, file_name):
    image = Image.fromarray(image_array)
    image.save(file_name)

def main():
    # Retrieve optional arguments
    num_arguments = len(sys.argv)
    input_file_name = sys.argv[1] if (num_arguments >= 2) else 'output.txt'
    output_image_name = sys.argv[2] if (num_arguments >= 3) else 'output.png'

    # Load input data
    data = load_data_from_file(input_file_name)

    # Process the data
    processed_data = process_data(data, False)

    # Create the output image
    create_image(processed_data, output_image_name)

if __name__ == '__main__':
    main()

