import random

def point_on_triangle(pt1, pt2, pt3):
    """
    Random point on the triangle with vertices pt1, pt2 and pt3.
    """
    s, t = sorted([random.random(), random.random()])
    return (s * pt1[0] + (t-s)*pt2[0] + (1-t)*pt3[0],
            s * pt1[1] + (t-s)*pt2[1] + (1-t)*pt3[1])

pt1 = (1, 1)
pt2 = (2, 4)
pt3 = (5, 2)
points = [point_on_triangle(pt1, pt2, pt3) for _ in range(10000)]

import matplotlib.pyplot as plt
x, y = zip(*points)
plt.scatter(x, y, s=0.1)
plt.show()
