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
 *  Convolution Filter Base class
 *
 */

#ifndef _CCCONVOLUTIONFILTER_HPP_
#define _CCCONVOLUTIONFILTER_HPP_

#include "CCImageReader.hpp"

// Image Format
enum class CCImageCVFilters {

    GAUSSIAN_BLUR,

    BOX_FILTER,

    MEDIAN_FILTER,
};

// Base Class
class CCImageConvolutionFilter {
    public:

    CCImageConvolutionFilter() : dimX(0), dimY(0) {}

    CCImageConvolutionFilter(int dX, int dY) : dimX(dX), dimY(dY) {} 

    CCImageConvolutionFilter(const CCImageConvolutionFilter &cv) : dimX(cv.dimX), dimY(cv.dimY) {}

    CCImageConvolutionFilter& operator=(const CCImageConvolutionFilter &cv) {
        dimX     = cv.dimX;
        dimY     = cv.dimY;
        return *this;
    }

    virtual ~CCImageConvolutionFilter() {}

    virtual void Run(CCImageReader &img) {}

    protected:

    int dimX;

    int dimY;
};
#endif
