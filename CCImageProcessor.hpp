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
 *  ImageProcessor Class
 *
 */

#ifndef _CCIMAGEPROCESSOR_HPP_
#define _CCIMAGEPROCESSOR_HPP_

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

    CCImageConvolutionFilter(const CCImageConvolutionFilter &cv) {
        dimX = cv.dimX;
        dimY = cv.dimY;
    } 

    CCImageConvolutionFilter& operator=(const CCImageConvolutionFilter &dv) {
        dimX     = dv.dimX;
        dimY     = dv.dimY;
        return *this;
    }

    virtual ~CCImageConvolutionFilter() {}

    virtual void Run(CCImageReader &img) {}

    protected:

    int dimX;

    int dimY;
};

enum class CCImageEDFilter {

    SOEBEL,
};

// Base Class
class CCImageDerivativeFilter {

    public:

    CCImageDerivativeFilter() : dimX(0), dimY(0), variance(0.0) {}

    CCImageDerivativeFilter(int dX, int dY, float var) : dimX(dX), dimY(dY), variance(var) {}

    CCImageDerivativeFilter(const CCImageDerivativeFilter &dv) {
        dimX     = dv.dimX;
        dimY     = dv.dimY;
        variance = dv.variance;
    }

    CCImageDerivativeFilter& operator=(const CCImageDerivativeFilter &dv) {
        dimX     = dv.dimX;
        dimY     = dv.dimY;
        variance = dv.variance;
        return *this;
    }

    virtual ~CCImageDerivativeFilter() {}

    virtual void Run(CCImageReader &img) {}

    protected:

    int dimX;

    int dimY;

    float variance;
};

// Image processor
class CCImageProcessor {

   public:

   CCImageProcessor(CCImageConvolutionFilter& cv, CCImageDerivativeFilter &dv) : cvFilter(cv), dvFilter(dv) {}

   CCImageProcessor(const CCImageProcessor &imgP) {
        //cvFilter = imgP.cvFilter;
        //dvFilter = imgP.dvFilter;
   }

   virtual ~CCImageProcessor() {}

   virtual void Run(void) {}

   private:

   CCImageConvolutionFilter cvFilter;

   CCImageDerivativeFilter dvFilter;
};

//
// Image Builder Processor
// Notes:
// delegate to Builder object
// The problem is that an object may have many options. The combination of each
// option would lead to a huge list of constructors for this class.
// So we will create a builder class.
class CCImageProcessorBuilder {
    
    public:

    // Note: requires default constructors for Filter classes
    CCImageProcessorBuilder() {}

    virtual ~CCImageProcessorBuilder() {}

    virtual CCImageProcessor build() {
        return CCImageProcessor(cvFilter, dvFilter);
    }

    virtual CCImageProcessorBuilder& addAveragingFilter(CCImageCVFilters type, int dimX, int dimY);

    virtual CCImageProcessorBuilder& addDerivativeFilter(CCImageEDFilter type, int dimX, int dimY, float variance);

    private:

    CCImageConvolutionFilter cvFilter;

    CCImageDerivativeFilter dvFilter;
};

#endif
