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
#include <vector>

#include "CCConvolutionFilter.hpp"
#include "CCGaussianFilter.hpp"
#include "CCDerivativeFilter.hpp"
#include "CCSoebelFilter.hpp"
#include "CCMorphologicalFilter.hpp"
#include "CCErosionFilter.hpp"
#include "CCFeatureExtractor.hpp"
#include "CCImageClassifier.hpp"
#include "CCImageReader.hpp"
#include "CCThresholding.hpp"

// Image processor
class CCImageProcessor {

   public:

   CCImageProcessor() {}

   CCImageProcessor(std::shared_ptr<CCImageConvolutionFilter> pCV,
        std::shared_ptr<CCImageDerivativeFilter> pDV) :
        pCV_(pCV), pDV_(pDV), pSD_(nullptr), pMF_(nullptr), pThresh_(nullptr) {}

   CCImageProcessor(std::shared_ptr<CCImageConvolutionFilter> pCV,
        std::shared_ptr<CCImageDerivativeFilter> pDV,
        std::shared_ptr<CCFeatureExtractor> pSD) :
        pCV_(pCV), pDV_(pDV), pSD_(pSD), pMF_(nullptr), pThresh_(nullptr) {}

   CCImageProcessor(std::shared_ptr<CCImageConvolutionFilter> pCV,
        std::shared_ptr<CCImageDerivativeFilter> pDV,
        std::shared_ptr<CCFeatureExtractor> pSD,
        std::shared_ptr<CCMorphologicalFilter> pMF) :
        pCV_(pCV), pDV_(pDV), pSD_(pSD), pMF_(pMF), pThresh_(nullptr) {}

   CCImageProcessor(std::shared_ptr<CCImageConvolutionFilter> pCV,
        std::shared_ptr<CCImageDerivativeFilter> pDV,
        std::shared_ptr<CCFeatureExtractor> pSD,
        std::shared_ptr<CCMorphologicalFilter> pMF,
        std::shared_ptr<CCThresholding> pThresh) :
        pCV_(pCV), pDV_(pDV), pSD_(pSD), pMF_(pMF), pThresh_(pThresh) {}

   CCImageProcessor(const CCImageProcessor &proc) :
        pCV_(proc.pCV_), pDV_(proc.pDV_), pSD_(proc.pSD_), pMF_(proc.pMF_), pThresh_(proc.pThresh_) {}

   virtual ~CCImageProcessor() {}

   virtual void Run(CCImageReader &img) {
       if (pMF_)
            pMF_->Run(img);

       if (pCV_)
            pCV_->Run(img);

       if (pDV_)
            pDV_->Run(img);

       if (pThresh_)
           pThresh_->Run(img);

       // debugging
       img.GetAllPixels(std::string("EDGE"));

       if (pSD_)
            pSD_->Run(img);
   }

   virtual bool Classify(CCImageReader &img, std::vector<int> exp) {
       if (pSD_) {
           auto features = pSD_->GetFeatures();
           return CCImageClassifier::DetectShapes(features, exp);
       }
       return false;
   }

   private:

   std::shared_ptr<CCImageConvolutionFilter> pCV_;

   std::shared_ptr<CCImageDerivativeFilter> pDV_;

   std::shared_ptr<CCFeatureExtractor> pSD_;

   std::shared_ptr<CCMorphologicalFilter> pMF_;

   std::shared_ptr<CCThresholding> pThresh_;
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
        return CCImageProcessor(pCV_, pDV_, pSD_, pMF_, pThresh_);
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
        addMorphFilter(int dimX, int dimY, int thresh) {
            pMF_.reset(new CCErosionFilter(dimX, dimY, thresh));
            return *this;
    }

    virtual CCImageProcessorBuilder&
        addFeatureExtractor(int distance) {
            pSD_.reset(new CCFeatureExtractor(distance));
            return *this;
    }

    virtual CCImageProcessorBuilder&
        addThresholding(int thresh) {
            pThresh_.reset(new CCThresholding(thresh));
            return *this;
    }

    private:

    std::shared_ptr<CCImageConvolutionFilter> pCV_;

    std::shared_ptr<CCImageDerivativeFilter> pDV_;

    std::shared_ptr<CCFeatureExtractor> pSD_;

    std::shared_ptr<CCMorphologicalFilter> pMF_;

    std::shared_ptr<CCThresholding> pThresh_;
};

#endif
