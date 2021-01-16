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

   static std::vector<int> CountVertices(std::list<Contour<int>> &approxContours) {
        std::vector<int> result;

        for (auto &c : approxContours) {

           int nr_vertices = c.getSize();
           if (nr_vertices <= 2)
               continue;

           CC_INFO("classifying object");
           CONSOLE_INFO("classifying object :")

           CC_INFO("Polygon ID ", c.getUUId(), "(", nr_vertices, ")");
           CONSOLE_INFO("Polygon ID", c.getUUId(), " (", nr_vertices, ")");
           
           std::cout << "[" << " ";
           for (auto &p : c.boundaryPixels) {
              std::cout << " (" << p.getX() << "," << p.getY() << ")" << ",";
              CC_INFO("CTOUR", c.getUUId(), p.getX(), p.getY());
           }
           std::cout << "]" << " " << std::endl;
           result.push_back(nr_vertices);
        }

        // Get rid of boundary square
        auto it = std::find(result.begin(), result.end(), 4);
        if (it != result.end())
            result.erase(it);

        return result;
   }

   static bool DetectShapes(std::list<Contour<int>> &approxContours,
                            std::vector<int> vertices) {
        std::vector<int> result;

        result = CCImageClassifier::CountVertices(approxContours);
        if (!result.size())
            goto error;

        for (int &v : vertices) {
            auto it = std::find(result.begin(), result.end(), v);
            if (it != result.end())
                result.erase(it);
            else
                goto error;
        }

        CC_INFO("MATCH");
        CONSOLE_INFO("MATCH");
        return true;

        error:
        CC_INFO("MISMATCH");
        CONSOLE_INFO("MISMATCH");
        return false;
   }
};
#endif
