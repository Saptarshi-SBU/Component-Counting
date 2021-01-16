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

#ifndef CCTHRESHOLDING_HPP_
#define CCTHRESHOLDING_HPP_

#include <string.h>

#include <list>
#include <cassert>
#include "CCPixel.hpp"
#include "CCPixelUtils.hpp"
#include "CCImageReader.hpp"

class CCThresholding {

    public:

    CCThresholding() {}

    CCThresholding(int dist_threshold) : dist_threshold_(dist_threshold) {}

    virtual ~CCThresholding() {}

    void Run(CCImageReader &img) {
        byte *src;
        int height, width;//, numChannels;

        src    = img.getDataBlob();
        height = img.getHeight();
        width  = img.getWidth();

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                Pixel<int> pixel(j, i);
                int index = pixel.getY() * width + pixel.getX();
                if ((byte) src[index] >= dist_threshold_)
                   src[index] = 255;
                else
                   src[index] = 0;
            }
        }
    }

    private:

    int dist_threshold_;
};
#endif
