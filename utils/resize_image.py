from PIL import Image
import numpy as np
import sys

img = Image.open( '../assets/dice_two.png' )

output_image = img.resize((410,410))
output_image.save('test.png')