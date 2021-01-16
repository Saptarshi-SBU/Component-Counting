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

TRIANGLE_DIR = '../images/triangles/'
SQUARE_DIR   = '../images/squares/'
CIRCLE_DIR   = '../images/circles/'

def TestSampleImage(path):
    im = cv2.imread(path, 0)
    edges = cv2.Canny(im, 100, 200)
    plt.subplot(121),plt.imshow(im, cmap = 'gray')
    plt.title('Original Image'), plt.xticks([]), plt.yticks([])
    plt.subplot(122),plt.imshow(edges, cmap = 'gray')
    plt.title('Edge Image'), plt.xticks([]), plt.yticks([])
    plt.imshow()
    plt.show()

def TestSampleFeatureSet(X, i):
    dimX = X.shape[0]
    dimY = X.shape[1]
    plt.imshow(X[i, :].reshape(dimX, dimY))
    plt.show()

def CreateTrainingData(path, y):
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

    #transform
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

def TrainAndPredict(X, Y):
    expected  = []
    predicted = []
    logreg = LogisticRegression(C=0.02, solver='lbfgs', multi_class='multinomial', max_iter=1000, random_state=0)
    X_train, X_test, Y_train, Y_test \
        = train_test_split(X, Y, test_size=0.2, random_state=1)
    print 'total size {} {}'.format(X.shape, Y.shape)
    print 'train size {} {}'.format(X_train.shape, Y_train.shape)
    print 'test size  {} {}'.format(X_test.shape, Y_test.shape)
    logreg.fit(X_train, Y_train)
    for i in range(0, X_test.shape[0]):
        expected  = np.append(expected, Y_test[i])
        predicted = np.append(predicted, logreg.predict([X_test[i, :]]))
    print("Classification report for classifier %s:\n%s\n"
          % (logreg, metrics.classification_report(expected, predicted)))
    print("Confusion matrix:\n%s" % metrics.confusion_matrix(expected, predicted))
    print logreg.score(X_test, Y_test)

def CreateFourShapesDataSet():
    [X1, Y1] = CreateTrainingData(TRIANGLE_DIR, 1)
    [X2, Y2] = CreateTrainingData(SQUARE_DIR,   2)
    [X3, Y3] = CreateTrainingData(CIRCLE_DIR,   3)
    X = np.concatenate((X1, X2, X3), 0)
    Y = np.concatenate((Y1, Y2, Y3), 0)
    np.save('img_ip_train.npy', X);
    np.save('img_op_train.npy', Y);
    return X, Y

def LoadMinstDataSet():    
    X = np.load('minst_ip_train.npy')
    Y = np.load('minst_op_train.npy')
    return X, Y

if __name__ == "__main__":
    X, Y = CreateFourShapesDataSet();
    TrainAndPredict(X, Y)
    X, Y = LoadMinstDataSet()
    TrainAndPredict(X, Y)
