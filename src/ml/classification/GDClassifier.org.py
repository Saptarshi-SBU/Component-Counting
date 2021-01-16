'''
   python module for multi-class classification based on cost function optimization

   -uses gradient descent for optimal theta finding
'''
import os
import cv2
import sklearn
import numpy as np
from sklearn.linear_model import SGDClassifier
from sklearn.linear_model import LogisticRegression
from sklearn.linear_model import LogisticRegressionCV

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

def GradientDescent(X, y, theta, learning_rate=0.01, iterations=1000):
    '''
    X    = Matrix of X with added bias units
    y    = Vector of Y
    theta=Vector of thetas np.random.randn(j,1)
    learning_rate 
    iterations = no of iterations
    
    Returns the final theta vector and array of cost history over no of iterations
    '''
    X = np.asarray(X, dtype=np.float64)
    y = np.asarray(y, dtype=np.float64)
    m = y.size;
    y = y.reshape(m, 1);
    cost_history = np.zeros(iterations)
    #print m
    #print X
    #print theta
    for it in range(iterations):
        prediction = sigmoid(np.dot(X, theta))
        #print 'predict {}/{}:{}'.format(prediction, prediction.shape[0], prediction.shape[1])
        assert prediction.shape[0] == y.shape[0]
        assert prediction.shape[1] == y.shape[1]
        theta = theta - learning_rate*(X.T.dot(np.absolute(prediction - y)))/m
        J, grad = CostFunction(theta, X, y, 0.2)
        print 'pred {} exp {} error {}cost {}'.\
            format(prediction, y, np.absolute(prediction - y), J)
    return theta, cost_history

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

    #J, grad = CostFunction(theta, X, Y, lmbda);

    return GradientDescent(X, Y, theta)

def predictOneVsAll(all_theta, X):
    '''
    PREDICT Predict the label for a trained one-vs-all classifier. The labels 
    are in the range 1..K, where K = size(all_theta, 1). 
      p = PREDICTONEVSALL(all_theta, X) will return a vector of predictions
      for each example in the matrix X. Note that X contains the examples in
      rows. all_theta is a matrix where the i-th row is a trained logistic
      regression theta vector for the i-th class. You should set p to a vector
      of values from 1..K (e.g., p = [1; 3; 1; 2] predicts classes 1, 3, 1, 2
      for 4 examples) 
    '''

    m = np.size(X, 1);
    num_labels = size(all_theta, 1);

    #You need to return the following variables correctly 
    p = np.zeros(np.size(X, 1), 1);
    p = np.transpose(p)

    #Add ones to the X data matrix
    X = np.concatenate((np.ones(m, 1), X), 1);

    for i in np.nditer(np.arange(m)):
            s = np.zeros(num_labels, 1);
            for j in np.nditer(np.arange(num_labels)):
                    theta = np.transpose(all_theta[j, :]);
                    s[j] = sigmoid(X[i, :] * theta);
            M, I = max(np.transpose(s), [], 2, 'linear');
            p[i] = I;


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
    Yy = Y[:, 0]
    Yy = np.transpose(Yy)
    theta = np.zeros((1, X.shape[1]), dtype=float)
    theta.fill(0.01)
    #print theta.shape[0], theta.shape[1]
    theta = np.transpose(theta)
    #print 'theta {}'.format(theta)
    #X = X / 255
    #GradientDescent(X, Yy, theta)
    #print (X[2,:])
    #print 'X {}'.format(X1)
    print 'Y {}'.format(Y[:, 0])
    clf = SGDClassifier(loss="log", penalty="l2", max_iter=100000, tol=0.0001)
    '''
    A = np.array(([1, 0, 1], [1, 1, 0]))
    print A
    B = np.array(([0, 1]))
    print B
    clf.fit(A, B)
    #clf.fit([[1, 0, 1], [1, 1, 0]], [0, 1])
    print clf.predict([[0, 0, 0]])
    print clf.predict([[1, 0, 1]])
    print clf.predict([[1, 1, 0]])
    print clf.predict([[1, 1, 1]])
    print clf.predict([[0, 1, 1]])
    clf.fit(X, np.transpose(Y[:, 0]))
    print clf.predict([X[5, :]])
    print clf.predict([X[125, :]])
    print clf.predict([X[256, :]])
    '''
    logreg = LogisticRegression(C=1e5, solver='lbfgs', multi_class='multinomial', max_iter=100)
    X[X == 255] = 0
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
    logreg.fit(X, Z)
    print X[5, :]
    print X[125, :]
    print X[5, :].size
    print Y[:,1]
    print Y[:,1].size

    for i in range(0, 99):
        print logreg.predict([X[i, :]])
        print i

    for i in range(0, 99):
        print logreg.predict([X[99 + i, :]])
        print i + 99

    print logreg.predict([X[5, :]])
    print logreg.predict([X[10, :]])
    print logreg.predict([X[15, :]])
    print logreg.predict([X[20, :]])
    print logreg.predict([X[25, :]])

    print logreg.predict([X[115, :]])
    print logreg.predict([X[125, :]])
    print logreg.predict([X[150, :]])
    print logreg.predict([X[175, :]])

    print logreg.predict([X[200, :]])
    print logreg.predict([X[210, :]])
    print logreg.predict([X[220, :]])
    print logreg.predict([X[256, :]])
    #print(logreg.coef_)
    #print(logreg.intercept_)
    import csv
    with open('eggs.csv', 'w') as csvfile:
        spamwriter = csv.writer(csvfile, delimiter=' ')
        spamwriter.writerow(X[5, :])
        spamwriter.writerow(X[125, :])
