'''
   python module for object shape classification based on linear regression
'''
import os
import cv2
import sklearn
import numpy as np
from sklearn import metrics, svm
from matplotlib import pyplot as plt
from sklearn.linear_model import LogisticRegression
from sklearn.model_selection import train_test_split

TRIANGLE_DIR_TRAIN = '../images/triangles_train/'
SQUARE_DIR_TRAIN   = '../images/squares_train/'
CIRCLE_DIR_TRAIN   = '../images/circles_train/'

TRIANGLE_DIR_TEST = '../images/triangles_test/'
SQUARE_DIR_TEST   = '../images/squares_test/'
CIRCLE_DIR_TEST   = '../images/circles_test/'

def TestSampleImage(path):
    im = cv2.imread(path, 0)
    edges = cv2.Canny(im, 100, 200)
    plt.subplot(121),plt.imshow(im, cmap = 'gray')
    plt.title('Original Image'), plt.xticks([]), plt.yticks([])
    plt.subplot(122),plt.imshow(edges, cmap = 'gray')
    plt.title('Edge Image'), plt.xticks([]), plt.yticks([])
    plt.imshow()
    plt.show()

def LoadImageDir(path, y):
    im = None
    X = None
    Y = []
    for (dpaths, dnames, filenames) in os.walk(path):
        for f in filenames:
            filepath = '{}{}'.format(dpaths, f)
            im = cv2.imread(filepath)
            im = cv2.Canny(im, 100, 200)
            im = im.reshape(1, -1)
            im = im.astype('float64')
            if X is None:
                X = im
            else:
                X = np.concatenate((X, im), 0)
            Y = np.append(Y, y)

    for (dpaths, dnames, filenames) in os.walk(path):
        for f in filenames:
            filepath = '{}{}'.format(dpaths, f)
            im = cv2.imread(filepath)
            im = cv2.flip(im, 0)
            im = cv2.Canny(im, 100, 200)
            im = im.reshape(1, -1)
            im = im.astype('float64')
            if X is None:
                X = im
            else:
                X = np.concatenate((X, im), 0)
            Y = np.append(Y, y)
    return X/255, Y

#TestSampleImage(path)
#>> plt.imshow(X[205, :].reshape(28,28))
#<matplotlib.image.AxesImage object at 0x49e6d10>
#>>> plt.show()
# from sklearn.datasets import fetch_openml
#X, y = fetch_openml('mnist_784', version=1, return_X_y=True)

if __name__ == "__main__":
    expected  = []
    predicted = []
    [X1, Y1] = LoadImageDir(TRIANGLE_DIR_TRAIN, 1)
    [X2, Y2] = LoadImageDir(SQUARE_DIR_TRAIN,   2)
    [X3, Y3] = LoadImageDir(CIRCLE_DIR_TRAIN,   3)
    X = np.concatenate((X1, X2, X3), 0)
    Y = np.concatenate((Y1, Y2, Y3), 0)
    X = np.load('minst_ip_train.npy')
    Y = np.load('minst_op_train.npy')
    logreg = LogisticRegression(C=0.02, solver='lbfgs', multi_class='multinomial', max_iter=1000, random_state=0)
    X_train, X_test, Y_train, Y_test \
        = train_test_split(X, Y, test_size=0.2, random_state=1)
    print X.shape, Y.shape
    print X_test.shape, Y_test.shape
    print X_train.shape, Y_train.shape
    logreg.fit(X_train, Y_train)
    #logreg.fit(X, Y)


    '''
    for i in range(0, X_test.shape[0]):
        #print logreg.predict([X[i, :]])
        expected  = np.append(expected, Y_test[i])
        predicted = np.append(predicted, logreg.predict([X_test[i, :]]))
    '''
    
    for i in range(0, X_train.shape[0]):
        #print logreg.predict([X[i, :]])
        expected  = np.append(expected, Y_train[i])
        predicted = np.append(predicted, logreg.predict([X_train[i, :]]))

    '''
    [X1, Y1] = LoadImageDir(TRIANGLE_DIR_TEST, 1)
    [X2, Y2] = LoadImageDir(SQUARE_DIR_TEST,   2)
    [X3, Y3] = LoadImageDir(CIRCLE_DIR_TEST,   3)
    X = np.concatenate((X1, X2, X3), 0)
    Y = np.concatenate((Y1, Y2, Y3), 0)
    for i in range(0, X3.shape[0]):
        print 'pred {}'.format(logreg.predict([X3[i, :]]))
        print Y3[i]
        expected  = np.append(expected, Y3[i])
        predicted = np.append(predicted, logreg.predict([X3[i, :]]))
    '''

    print("Classification report for classifier %s:\n%s\n"
          % (logreg, metrics.classification_report(expected, predicted)))
    print("Confusion matrix:\n%s" % metrics.confusion_matrix(expected, predicted))

    #np.save('img_ip_train.npy', X);
    #np.save('img_op_train.npy', Y);
