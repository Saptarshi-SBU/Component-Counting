'''
   python module for cost-function computation using numpy
'''
import os
import cv2
import sklearn
import numpy as np

def sigmoid(im):
    print type(im)
    return 1.0/(1.0 + np.exp(-im))

def CostFunction(theta, X, y, lmbda):
    '''
    LRCOSTFUNCTION
       computes the cost of using
       theta as the parameter for regularized logistic regression and the
       gradient of the cost w.r.t. to the parameters. 
    '''

    J = 0;
    X = np.asarray(X)

    #number of training examples
    y = np.asarray(y)
    m = y.size;

    theta = np.asarray(theta)
    print 'theta {}'.format(theta)
    grad = np.zeros(theta.size);

    thetaX = X.dot(theta);
    #print thetaX.shape[0], X.shape[0]
    #print thetaX.shape[1], X.shape[1]

    print 'thetaX {}'.format(thetaX)
    actvFunc = sigmoid(thetaX);
    print np.size(actvFunc);
    #print 'actvFunc {}'.format(actvFunc)

    J = (sum (-y*np.log(actvFunc) - (1 - y)*np.log(1 - actvFunc)) / m) \
            + (lmbda / ( 2 * m)) * np.sum(np.square(theta[1:len(theta)]));

    #print (actvFunc - np.transpose(y))
    grad =  (np.transpose(X).dot(actvFunc - y) + \
            lmbda*np.concatenate(([[0]], theta[1 : theta.size]), 0))/m;
    return J, grad

def CostFunctionTest():
    '''
       Unit Test function for CostFunction
    '''
    #features
    X = [[1, 2, 3, 4, 5], [2, 3, 4, 5, 6], [3, 4, 5, 6, 7]]

    #result
    Y = [[1, 0, 1]]
    Y = np.transpose(Y)

    #poly
    theta = [[0.0, 0.1, 0.2, 0.1, 0.2, 0.1]]
    theta = np.transpose(theta)

    #regularization parameter
    lmbda = 0.2;

    rows = np.size(X, 0);

    cols = np.size(X, 1);

    X = np.concatenate((np.ones((rows, 1)), X), 1);

    J, grad = CostFunction(theta, X, Y, lmbda);

if __name__ == "__main__":
    CostFunctionTest()
