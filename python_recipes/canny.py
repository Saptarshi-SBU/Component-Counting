import cv2
import numpy as np
from matplotlib import pyplot as plt

img = cv2.imread('download3.png',0)
edges = cv2.Canny(img,100,200)
cv2.imwrite('download4.png', edges)
