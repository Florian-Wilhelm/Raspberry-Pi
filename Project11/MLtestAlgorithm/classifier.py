# test ML algorithm w/ classifier for MLX90642 IR array test data, in order to predict whether a burglar is in the proximity
# the formulas are taken from literature sources

import numpy as np

def sigmoid(z):
	return 1 / (1 + np.exp(-z))

def forward(X, w):	
	weighted_sum = np.matmul(X, w)
	return sigmoid(weighted_sum)
	
def classify(X, w):	
	return np.round(forward(X, w))

def loss(X, Y, w):
	y_hat = forward(X, w)
	first_term = Y * np.log(y_hat)
	second_term = (1 - Y) * np.log(1 - y_hat)
	return -np.average(first_term + second_term)	

def gradient(X, Y, w):
	return np.matmul(X.T, (forward(X, w) - Y)) / X.shape[0]
	
def train(X, Y, iterations, lr):
	w = np.zeros((X.shape[1], 1))
	for i in range(iterations):
	    print("Iteration %4d => Loss: %.20f" % (i, loss(X, Y, w)))	    
	    w -= gradient(X, Y, w) * lr
	return w
	
def test(X, Y, w):
	total_examples = X.shape[0]
	correct_results = np.sum(classify(X, w) == Y)
	success_percent = correct_results * 100 / total_examples
	print("\nSuccess: %d/%d (%.2f%%)" % (correct_results, total_examples, success_percent))
		
# rendering of the data
x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48, x49, x50, x51, x52, x53, x54, x55, x56, x57, x58, x59, x60, y = np.loadtxt("testData.txt", skiprows=1, unpack=True)
X = np.column_stack((np.ones(x1.size), x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48, x49, x50, x51, x52, x53, x54, x55, x56, x57, x58, x59, x60))
Y = y.reshape(-1, 1)

# train the system
w = train(X, Y, iterations=1000, lr=0.001)

# test the data
test(X, Y, w)

print(w) # printing the weights
