# Component-Counting
Component counting based on image processing techniques

The project explores several approaches for implementing object detection
starting with legacy based polygonal approximation techniques to machine
learning techniques.

polygon approximation : uses geometric algorithms like convex hull to estimate
the type of polygon. Some amount of image preprocessing like blurring, edge-detection
is required

machine learning : using modern deep learning techniques. This is work in progress.

# Setup

Below are the steps for setting up the work environment and get the jupyter notebook running
This applies to Linux Ubuntu18, should work for higher ubuntu versions as well.

Download Anaconda https://www.anaconda.com/products/individual
for installing the conda package manager

A)
 1.  bash Anaconda3-2020.11-Linux-x86_64.sh
 2.  source ~/.bashrc
 3.  conda info
 4.  history
 5.  conda create -n py36 python=3.6
 6.  conda activate py36
 7.  conda install pytorch torchvision torchaudio cudatoolkit=10.2 opencv -c pytorch
 8.  conda install -c conda-forge notebook

B)
 1. sudo apt install git
 2. git clone https://github.com/Saptarshi-SBU/Component-Counting.git
 3. cd Component-Counting/src/ml/object_detection/
 4. conda install -c anaconda pandas
 5. conda install -c conda-forge matplotlib
 6. jupyter notebook

# Running training & validation with our dataset
 
 Below are the steps to perform a sample training & inference
  
 1. epochs = 8
 2. batch_size = 4
 3. md = train_dataset('data', epochs, batch_size, max_files=16)
 4. eval_dataset(md, 'data', max_files=16)
