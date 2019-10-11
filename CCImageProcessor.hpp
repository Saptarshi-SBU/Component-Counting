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

#include <memory>

#include "CCConvolutionFilter.hpp"
#include "CCGaussianFilter.hpp"

#include "CCDerivativeFilter.hpp"
#include "CCSoebelFilter.hpp"

#include "CCShapeDetector.hpp"

#include "CCImageReader.hpp"

// Image processor
class CCImageProcessor {

   public:

   CCImageProcessor() {}

   CCImageProcessor(std::shared_ptr<CCImageConvolutionFilter> pCV,
        std::shared_ptr<CCImageDerivativeFilter> pDV) :
        pCV_(pCV), pDV_(pDV) {}

   CCImageProcessor(std::shared_ptr<CCImageConvolutionFilter> pCV,
        std::shared_ptr<CCImageDerivativeFilter> pDV,
        std::shared_ptr<CCShapeDetector> pSD) :
        pCV_(pCV), pDV_(pDV), pSD_(pSD) {}

   CCImageProcessor(const CCImageProcessor &proc) :
        pCV_(proc.pCV_), pDV_(proc.pDV_), pSD_(proc.pSD_) {}

   virtual ~CCImageProcessor() {}

   virtual void Run(CCImageReader &img) {
       if (pCV_)
            pCV_->Run(img);
       if (pDV_)
            pDV_->Run(img);
       if (pSD_)
           pSD_->Run(img);
           //pSD_->GetAllPixels(img);
   }

   private:

   std::shared_ptr<CCImageConvolutionFilter> pCV_;

   std::shared_ptr<CCImageDerivativeFilter> pDV_;

   std::shared_ptr<CCShapeDetector> pSD_;
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
        return CCImageProcessor(pCV_, pDV_, pSD_);
    }

    virtual CCImageProcessorBuilder&
        addGaussianFilter(int dimX, int dimY, float variance) {
            pCV_.reset(new CCGaussianFilter(dimX, dimY, variance));
            return *this;
    }

    virtual CCImageProcessorBuilder&
        addSoebelFilter(int dimX, int dimY, float variance) {
            pDV_.reset(new CCSoebelFilter(dimX, dimY, variance));
            return *this;
    }

    virtual CCImageProcessorBuilder&
        addShapeDetector(int distance) {
            pSD_.reset(new CCShapeDetector(distance));
            return *this;
    }

    private:

    std::shared_ptr<CCImageConvolutionFilter> pCV_;

    std::shared_ptr<CCImageDerivativeFilter> pDV_;

    std::shared_ptr<CCShapeDetector> pSD_;
};

#endif
