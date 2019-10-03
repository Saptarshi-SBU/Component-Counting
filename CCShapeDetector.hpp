/*
 * Copyright <2019> Saptarshi Sen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef CCSHAPEDETECTOR_HPP_
#define CCSHAPEDETECTOR_HPP_

#include <string.h>

#include <list>
#include "CCPixel.hpp"
#include "CCPixelUtils.hpp"
#include "CCContour.hpp"
#include "CCContourFinding.hpp"
#include "CCPolygonApproximation.hpp"
#include "CCImageReader.hpp"

class CCShapeDetector {

    public:

    CCShapeDetector() {}

    CCShapeDetector(int dist_threshold) : dist_threshold_(dist_threshold) {}

    virtual ~CCShapeDetector() {}

    template<class T>
    int countVertices(const Contour<T> contour) {
        int num;
        std::list<Pixel<T>> polyPoints, boundaryPixelsApprox;

        for (auto &p : contour.boundaryPixels)
            polyPoints.push_back(p);
        polyPoints.sort(ComparatorLesserPixel<T>());

        boundaryPixelsApprox.clear();
        num = approxPolyDP<T>(polyPoints, *polyPoints.begin(),
                *polyPoints.rbegin(), boundaryPixelsApprox, dist_threshold_);
        boundaryPixelsApprox.push_back(*polyPoints.begin());
        boundaryPixelsApprox.push_back(*polyPoints.rbegin());
        printf("num=%d, vertices=%lu\n", num, boundaryPixelsApprox.size());
        return num;
    }

    void Run(CCImageReader &img) {
        byte *src;
        int height, width, numChannels;
        std::list<Contour<int>> contours_list;

        src    = img.getDataBlob();
        height = img.getHeight();
        width  = img.getWidth();
        numChannels = img.getNumChannels();

        byte *dst = new byte[height * width * numChannels];
        assert(dst);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                Pixel<int> pixel(j, i);
                Contour<int> contour =
                    MooreNeighbourhoodStrategy<int>(src, pixel, height, width, dst, contours_list);

                if (contour.empty())
                    continue;

                contours_list.push_back(contour);
                if (countVertices<int>(contour) == 1)
                    contour.drawContourApprox2(dst, width, height);
            }
            memcpy(src, dst, sizeof(byte) * width * height * numChannels);
            delete dst;
        }
    }

    private:

    int dist_threshold_;
};
#endif
