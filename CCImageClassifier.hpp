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
 *  ImageClassifier Class
 *
 */

#ifndef _CCIMAGECLASSIFIER_HPP_
#define _CCIMAGECLASSIFIER_HPP_

#include <memory>
#include <vector>
#include <algorithm>

#include "CCConvolutionFilter.hpp"
#include "CCGaussianFilter.hpp"
#include "CCDerivativeFilter.hpp"
#include "CCSoebelFilter.hpp"
#include "CCFeatureExtractor.hpp"
#include "CCImageReader.hpp"

// Image processor
class CCImageClassifier {

   public:

   CCImageClassifier() {}

   ~CCImageClassifier() {}

   static bool DetectShapes(std::list<Contour<int>> &featureVector,
                            std::vector<int> &expected) {
        std::vector<int> result;

        for (auto &c : featureVector) {
           unsigned int nr_vertices = c.getSize();

           if (nr_vertices <= 2)
               continue;

           std::cout << "classifying object :" << std::endl;
           CC_DEBUG("classifying object");

           std::cout << "Polygon ID " << c.getUUId() << " (" << nr_vertices << ")" << std::endl;
           CC_DEBUG("Polygon ID ", c.getUUId(), "(", nr_vertices, ")");
           
           std::cout << "[" << " ";
           for (auto &p : c.boundaryPixels) {
              std::cout << " (" << p.getX() << "," << p.getY() << ")" << ",";
              CC_DEBUG("CTOUR Pixel", c.getUUId(), p.getX(), p.getY());
           }
           std::cout << "]" << " " << std::endl;
           result.push_back(nr_vertices);
        }

        std::sort(result.begin(), result.end());
        std::sort(expected.begin(), expected.end());

        if (std::equal(result.begin(), result.end(), expected.begin())) {
            std::cout << "MATCH" << std::endl;
            CC_DEBUG("MATCH");
            return true;
        } else {
            std::cout << "MISMATCH" << std::endl;
            CC_DEBUG("MISMATCH");
            return false;
        }
   }

   private:
};
#endif
