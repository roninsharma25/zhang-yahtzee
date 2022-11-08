from PIL import Image
import numpy as np

thresholded_image_array = []

with open('outputfile.txt', 'r') as f:
        for line in f.readlines():
            row = [int(value) for value in line.split(' ')[:-1]]
            thresholded_image_array.append(row)

print(thresholded_image_array)

img = Image.fromarray(np.array(thresholded_image_array), 'L')
img.save('output.jpg')