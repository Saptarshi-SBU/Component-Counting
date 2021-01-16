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
 *  Soebel Filter Class
 *
 */

#ifndef _CCSOEBELFILTER_HPP_
#define _CCSOEBELFILTER_HPP_

#include <cmath>
#include "CCDerivativeFilter.hpp"
#include "CCImageReader.hpp"

class CCSoebelFilter : public CCImageDerivativeFilter {

    public:

    CCSoebelFilter() : CCImageDerivativeFilter(3, 3, 1.0) {
    }

    CCSoebelFilter(int dX, int dY, float var) : CCImageDerivativeFilter(dX, dY, var) {
    }

    CCSoebelFilter(const CCSoebelFilter &dv) : CCImageDerivativeFilter(dv) {
        variance = dv.variance;
    }

    CCSoebelFilter& operator=(const CCSoebelFilter &dv) {
        CCImageDerivativeFilter::operator=(dv);
        return *this;
    }

    virtual ~CCSoebelFilter() {
    }

    virtual void Run(CCImageReader &img) {
        int width  = img.getWidth();
        int height = img.getHeight();
        int numChannel = img.getNumChannels();
        uint8_t *imgData = img.getDataBlob();
        uint8_t *imgDst = new uint8_t[width * height * numChannel];

        //dbg_printf ("width :%d height :%d numChannels :%d\n",
        //    width, height, numChannel);

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {

                int a11 = -1, a12 = -1, a13 = -1;
                int a21 = -1, a22 = -1, a23 = -1;
                int a31 = -1, a32 = -1, a33 = -1;

                int boundary = 0;
        
                // y - boundary cases
                if (y - 1 < 0) {
                    a11 = a12 = a13 = 0;
                    boundary = 1;
                }

                if (y + 1 >= height) {
                    a31 = a32 = a33 = 0;
                    boundary = 1;
                }

                // x - boundary cases
                if (x - 1 < 0) {
                    a11 = a21 = a31 = 0;
                    boundary = 1;
                }

                if (x + 1 >= width) {
                    a13 = a23 = a33 = 0;
                    boundary = 1;
                }

                if (a11 < 0)
                    a11 = imgData[(y - 1) * width + x - 1];
                if (a12 < 0)
                    a12 = imgData[(y - 1) * width + x];
                if (a13 < 0)
                    a13 = imgData[(y - 1) * width + x + 1];
                if (a21 < 0)
                    a21 = imgData[y * width + x - 1];
                if (a23 < 0)
                    a23 = imgData[y * width + x + 1];
                if (a31 < 0)
                    a31 = imgData[(y + 1) * width + x - 1];
                if (a32 < 0)
                    a32 = imgData[(y + 1) * width + x];
                if (a33 < 0)
                    a33 = imgData[(y + 1) * width + x + 1];

                int gx = a13 - a11 + 2*(a23 - a21) + a33 - a31;
                int gy = a31 - a11 + 2*(a32 - a12) + a33 - a13;

                if (boundary)
                    a22 = 0;
                else
                    a22 = sqrtf(gx*gx + gy*gy);

                imgDst[y * width + x] = a22;
                //dbg_printf("(%d, %d) :%d %d \n", y, x, gx, gy);
                //dbg_printf("(%d, %d) :a11=%d a12=%d a13=%d a21=%d a22=%d a23=%d a31=%d a32=%d a33=%d\n",
                //    y, x, a11, a12, a13, a21, a22, a23, a31, a32, a33);
            }
        }

        memcpy(imgData, imgDst, width * height * numChannel);
        delete imgDst;
    }
};
#endif
