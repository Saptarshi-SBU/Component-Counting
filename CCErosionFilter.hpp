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
 *  Erosion Filter
 *
 */

#ifndef _CCEROSIONFILTER_HPP_
#define _CCEROSIONFILTER_HPP_

#include "CCMorphologicalFilter.hpp"

class CCErosionFilter : public CCMorphologicalFilter {

    public:

    CCErosionFilter() : CCMorphologicalFilter() {
    }

    CCErosionFilter(int m, int n) : CCMorphologicalFilter(m, n) {
    }

    virtual ~CCErosionFilter() {
    }

    virtual void Run(CCImageReader &img) {
        int width  = img.getWidth();
        int height = img.getHeight();
        int numChannel = img.getNumChannels();
        uint8_t *imgData = img.getDataBlob();
        uint8_t *imgDst = new uint8_t[width * height * numChannel];

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                bool fmatch = true;

                // boundary cases
                if ((y - m_/2 < 0) || (y + m_/2 >= height) ||
                    (x - n_/2 < 0) || (x + n_/2 >= width))  {
                    continue;
                }

                // iterate structural element
                for (int i = y - m_/2; i <= y + m_/2; i++) {
                    for (int j = x - n_/2; j <= x + n_/2; j++) {
                        if (imgData[i * width + j] == 0) { // no full match
                            fmatch = false;
                            goto out;
                        }
                    }
                }

             out:
                if (fmatch) {
                    //std::cout << x << ":" << y << std::endl;
                    imgDst[y * width + x] = 255;
                }
            }
        }

        memcpy(imgData, imgDst, width * height * numChannel);
        delete imgDst;
        std::cout << __func__ << std::endl;
    }
};
#endif
