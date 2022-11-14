from PIL import Image
import numpy as np

output_array = []

with open('output.txt', 'r') as f:
    output_array = f.readlines()[0].strip()

print(output_array)

#output_array = np.array(output_array)

#image = Image.fromarray(output_array)
#image.save('output.jpg')

if __name__ == '__main__':
    main()

