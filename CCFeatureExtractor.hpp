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

#ifndef CCFEATUREEXTRACTOR_HPP_
#define CCFEATUREEXTRACTOR_HPP_

#include <string.h>

#include <list>
#include <cassert>
#include "CCPixel.hpp"
#include "CCPixelUtils.hpp"
#include "CCImageReader.hpp"
#include "CCContour.hpp"
#include "CCContourTracing.hpp"

class CCFeatureExtractor {

    public:

    CCFeatureExtractor() {}

    CCFeatureExtractor(int dist_threshold) : dist_threshold_(dist_threshold) {}

    virtual ~CCFeatureExtractor() {}

    void Run(CCImageReader &img) {
        byte *src;
        int height, width, numChannels;

        src    = img.getDataBlob();
        height = img.getHeight();
        width  = img.getWidth();
        numChannels = img.getNumChannels();

        byte *dst = new byte[height * width * numChannels];
        memset(dst, 0, sizeof(byte) * width * height * numChannels);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                Pixel<int> pixel(i, j);
                Contour<int> contour =
                    BorderFollowingStrategy<int>(src, pixel, height, width, dst, contours_list);
                if (contour.empty())
                    continue;
                contour.makeConvexHull();
                contour.ApproxPoly(dist_threshold_);
                contour.drawContour(dst, width, height);
                //contour.drawContourApprox(dst, width, height);
                contours_list.push_back(contour);
            }
        }

        memcpy(src, dst, sizeof(byte) * width * height * numChannels);
        delete dst;
    }

    std::list<Contour<int>> GetFeatures(void) {
        return contours_list;
    }

    void GetAllPixels(CCImageReader &img) {
        byte *src;
        int height, width;

        src    = img.getDataBlob();
        height = img.getHeight();
        width  = img.getWidth();

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                Pixel<int> pixel(j, i);
                int index = i * width + j;
                if ((byte) src[index] == 255)
                    printf("REALIMG %d %d %u\n", j, i, (byte) src[index]);
            }
        }
    }

    private:

    int dist_threshold_;

    std::list<Contour<int>> contours_list;

};
#endif
