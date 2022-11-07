from PIL import Image
import numpy as np

img = Image.open( 'assets/dice.png' )
img.load()
data = np.asarray( img, dtype="int32" )
print(data)
