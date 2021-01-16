'''
    python module for training and testing MINST data-set
'''
from sklearn.datasets import fetch_openml
from sklearn.preprocessing import StandardScaler
from sklearn.neural_network import MLPClassifier
from sklearn.model_selection import train_test_split
from sklearn.metrics import classification_report,confusion_matrix

X, y = fetch_openml('mnist_784', version=1, return_X_y=True)

X_train, X_test, y_train, y_test = train_test_split(X, y)

scaler = StandardScaler()
scaler.fit(X_train)

X_train = scaler.transform(X_train)
X_test = scaler.transform(X_test)

mlp = MLPClassifier(hidden_layer_sizes=(13,13,13),max_iter=500, verbose=1)
mlp.fit(X_train,y_train)
predictions = mlp.predict(X_test)

print(classification_report(y_test,predictions))
print(confusion_matrix(y_test,predictions))
