/*
 * MIT License
 *
 * Copyright (c) 2019 Saptarshi Sen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *
 *  Gaussian Filter
 *
 */

#ifndef _CCGAUSSIANFILTER_HPP_
#define _CCGAUSSIANFILTER_HPP_

#include <cmath>
#include "CCConvolutionFilter.hpp"

static float* CreateGaussianKernel(const int kernelWidth=5, const float kernelSigma=1.0) {
    float *A = new float[kernelWidth];
    for (int x = -(kernelWidth - 1)/2, i = 0; x <= (kernelWidth + 1)/2; x++, i++){
        const float variance  = std::pow(kernelSigma, 2);
        const float distance  = expf(-(std::pow(x, 2)/(2 * variance)));
        const float kernel    = distance / sqrt((2 * M_PI * variance));
        A[i] = kernel;
    }
    return A;
}

class CCGaussianFilter : public CCImageConvolutionFilter {

    public:

    CCGaussianFilter() : CCImageConvolutionFilter(5, 0), variance(1.0) {
        A = CreateGaussianKernel(dimX, variance);
    }

    CCGaussianFilter(int dX, int dY, float var) : CCImageConvolutionFilter(dX, dY), variance(var) {
        A = CreateGaussianKernel(dimX, variance);
    }

    CCGaussianFilter(const CCGaussianFilter &cv) : CCImageConvolutionFilter(cv) {
        variance = cv.variance;
        A = CreateGaussianKernel(dimX, variance);
    }

    CCGaussianFilter& operator=(const CCGaussianFilter &cv) {
        CCImageConvolutionFilter::operator=(cv);
        variance = cv.variance;
        A = CreateGaussianKernel(dimX, variance);
        return *this;
    }

    virtual ~CCGaussianFilter() {
        if (A)
            delete A;
        A = nullptr;
    }

    void DumpFilter(float *A) {
        for (int i = 0; i < dimX; i++)
            printf ("%f  ", A[i]);
        printf("\n");
    }

    virtual void Run(CCImageReader &img) {
        int width  = img.getWidth();
        int height = img.getHeight();
        int numChannel = img.getNumChannels();
        uint8_t *imgData = img.getDataBlob();

        printf ("width :%d height :%d numChannels :%d\n",
            width, height, numChannel);

        float *storeBuffer = new float[width * height * numChannel];

        for (int r = 0; r < height; r++) {
            for (int c = 0; c < width; c++) {
                float accum = 0;
                for (int j = 0, k = c - dimX/2; j < dimX; j++, k++) {
                    if ((k < 0) || (k >= width))
                        continue;
                    accum += (A[j] * float(imgData[r * width + k]));
                }
                storeBuffer[r * width + c] = accum;
            }
        }

        for (int c = 0; c < width; c++) {
            for (int r = 0; r < height; r++) {
                float accum = 0;
                for (int j = 0, k = r - dimY/2; j < dimY; j++, k++) {
                    if ((k < 0) || (k >= height))
                        continue;
                    accum += (A[j] * storeBuffer[k * width + c]);
                }
                imgData[r * width + c] = std::min<uint8_t>(static_cast<uint8_t>(accum), 255);
            }
        }

        delete storeBuffer;
    }

    private:

    float *A {nullptr};

    float variance;
};
#endif
