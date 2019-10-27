import sys
import cv2
import numpy as np

print sys.argv[1]
img = cv2.imread(sys.argv[1], 0)
ret, thresh = cv2.threshold(img, 127, 255, 0)
contours, hierarchy = cv2.findContours(thresh, 1, 2)
for c in contours:
    area = cv2.contourArea(c)
    ep = 0.1 * cv2.arcLength(c, True);
    approx = cv2.approxPolyDP(c, ep, True);
    print 'nr_vertices: {}'.format(len(approx))
