'''
   python module for multi-class classification based on cost function optimization

   -uses gradient descent for optimal theta finding
'''
import os
import cv2
import sklearn
import numpy as np
from sklearn import datasets, svm, metrics
from sklearn.linear_model import SGDClassifier
from sklearn.linear_model import LogisticRegression
from sklearn.linear_model import LogisticRegressionCV

def PrepareData(path, y):
    c = 0
    X = []
    Y = []
    for (dpaths, dnames, filenames) in os.walk(path):
        for f in filenames:
            filepath = '{}{}'.format(dpaths, f)
            im = cv2.imread(filepath)
            im = cv2.cvtColor(im, cv2.COLOR_BGR2GRAY)
            im = im.astype('float64')
            #print im.shape[0], im.flatten[1]
            flim = im.flatten()
            X = np.append(X, flim);
            Y = np.append(Y, y)
            flipVertical = cv2.flip(im, 0)
            flipim = flipVertical.flatten()
            X = np.append(X, flipim);
            Y = np.append(Y, y)
            c = c + 1
    n = X.size / c
    X = X.reshape(c, n)
    n = Y.size / c 
    Y = Y.reshape(c, n)
    return X, Y

if __name__ == "__main__":
    path = '../images/triangles/'
    [X1, Y1] = PrepareData(path, [1, 0, 0])
    print '{}/{}:{}/{}'.format(X1.shape[0], X1.shape[1], Y1.shape[0], Y1.shape[1])
    path = '../images/triangles/'
    [X2, Y2] = PrepareData(path, [0, 2, 0])
    print '{}/{}:{}/{}'.format(X2.shape[0], X2.shape[1], Y2.shape[0], Y2.shape[1])
    path = '../images/circles/'
    [X3, Y3] = PrepareData(path, [0, 0, 3])
    print '{}/{}:{}/{}'.format(X3.shape[0], X3.shape[1], Y3.shape[0], Y3.shape[1])
    X = np.concatenate((X1, X2, X3), 0)
    Y = np.concatenate((Y1, Y2, Y3), 0)
    print '{}/{}:{}/{}'.format(X.shape[0], X.shape[1], Y.shape[0], Y.shape[1])

    logreg = LogisticRegression(C=1e5, solver='lbfgs', multi_class='multinomial', max_iter=100)
    #logreg = LogisticRegression(C=1e5, penalty='l2', solver='newton-cg', multi_class='multinomial', max_iter=1000)
    #logreg = LogisticRegression(C=0.02, penalty='l2', solver='liblinear', max_iter=1000)
    #logreg = LogisticRegression(C=0.02, penalty='l2', solver='newton-cg', multi_class='multinomial', max_iter=1000)
    #logreg = LogisticRegressionCV(solver='newton-cg', max_iter=1000)

    Z = []
    for i in range(0, 99):
        Z = np.append(Z, 1)
    print 'Z {}'.format(Z)

    for i in range(0, 99):
        Z = np.append(Z, 2)
    print 'Z {}'.format(Z)

    for i in range(0, 100):
        Z = np.append(Z, 3)
    print 'Z {}'.format(Z)

    print Z

    #logreg.fit(X, Y[:, 1])
    #logreg = svm.SVC(gamma=0.001)
    logreg.fit(X, Z)

    expected = []
    predicted = []

    print logreg.predict([X[5, :]])
    expected  = np.append(expected, 1)
    predicted = np.append(predicted, logreg.predict([X[5, :]]))

    print logreg.predict([X[10, :]])
    expected  = np.append(expected, 1)
    predicted = np.append(predicted, logreg.predict([X[10, :]]))

    print logreg.predict([X[15, :]])
    expected  = np.append(expected, 1)
    predicted = np.append(predicted, logreg.predict([X[15, :]]))

    print logreg.predict([X[20, :]])
    expected  = np.append(expected, 1)
    predicted = np.append(predicted, logreg.predict([X[20, :]]))

    print logreg.predict([X[115, :]])
    expected  = np.append(expected, 2)
    predicted = np.append(predicted, logreg.predict([X[115, :]]))

    print logreg.predict([X[125, :]])
    expected  = np.append(expected, 2)
    predicted = np.append(predicted, logreg.predict([X[125, :]]))

    print logreg.predict([X[150, :]])
    expected  = np.append(expected, 2)
    predicted = np.append(predicted, logreg.predict([X[150, :]]))

    print logreg.predict([X[175, :]])
    expected  = np.append(expected, 2)
    predicted = np.append(predicted, logreg.predict([X[175, :]]))

    print logreg.predict([X[200, :]])
    expected  = np.append(expected, 3)
    predicted = np.append(predicted, logreg.predict([X[200, :]]))

    print logreg.predict([X[210, :]])
    expected  = np.append(expected, 3)
    predicted = np.append(predicted, logreg.predict([X[210, :]]))

    print logreg.predict([X[220, :]])
    expected  = np.append(expected, 3)
    predicted = np.append(predicted, logreg.predict([X[220, :]]))

    print logreg.predict([X[256, :]])
    expected  = np.append(expected, 3)
    predicted = np.append(predicted, logreg.predict([X[256, :]]))

    print("Classification report for classifier %s:\n%s\n"
          % (logreg, metrics.classification_report(expected, predicted)))
    print("Confusion matrix:\n%s" % metrics.confusion_matrix(expected, predicted))
