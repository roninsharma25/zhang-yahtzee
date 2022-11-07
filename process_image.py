from PIL import Image
import numpy as np
import sys

np.set_printoptions(threshold=sys.maxsize)

img = Image.open( 'assets/dice.png' )
img.load()
data = np.asarray( img, dtype="int32" )

hex_array = [["0x"]*data.shape[1]]*data.shape[0]

for x in range(data.shape[0]):
    for y in range(data.shape[1]):
        for z in data[x][y]:
            temp = hex_array[x][y] + hex(z).lstrip("0x")
            hex_array[x][y] = temp    
    break
    


original_stdout = sys.stdout # Save a reference to the original standard output
with open('filename.txt', 'w') as f:
    sys.stdout = f # Change the standard output to the file we created.
    print(np.asarray(hex_array))
    sys.stdout = original_stdout # Reset the standard output to its original value