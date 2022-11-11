from PIL import Image
import numpy as np
import sys

np.set_printoptions(threshold=sys.maxsize)

img = Image.open( 'assets/dice.png' ).convert('L')
img.load()
data = np.asarray( img, dtype="int32" )
print('DATA')

print(data)

hex_array = []

for col in data:
    temp = []
    for i in range(len(col)):
        if (len(temp) < 4):
            temp.append(hex(col[i])[2:])
        else:
            hex_array.append('0x' + ''.join(temp))
            temp = []

print(hex_array)


original_stdout = sys.stdout # Save a reference to the original standard output
with open('filename.txt', 'w') as f:
    sys.stdout = f # Change the standard output to the file we created.
    print(np.asarray(hex_array))
    sys.stdout = original_stdout # Reset the standard output to its original value

# format the output
with open('output_formatted.txt', 'w') as f:
    f.write(str(data.shape[0]) + ',' + str(data.shape[1]) + '\n')
    for row in hex_array:
        f.write(row + '\n')