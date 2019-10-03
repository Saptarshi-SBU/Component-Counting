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

#ifndef CCONTOURFINDING_HPP_
#define CCONTOURFINDING_HPP_

#include <list>
#include "CCPixel.hpp"
#include "CCPixelUtils.hpp"
#include "CCContour.hpp"
#include "CCImageReader.hpp"

#define CONTOUR_THRESHOLD_SIZE 100

constexpr int PixelDirection::dirx[];

constexpr int PixelDirection::diry[];

enum contourTrackState {
    INIT,
    DONE,
};

template<class T>
static bool IsPresentInActiveContours(const Pixel<T> &pixel, const std::list<Contour<T>> &contours_list) {
    for (auto &c : contours_list) {
        if (!c.empty() && c.FindPixel(pixel))
            return true;
    }
    return false;
}

//core routine
template<class T>
static Contour<T>
MooreNeighbourhoodStrategy(byte *Img, 
                           const Pixel<T> &start_pixel, int ImgHeight, int ImgWidth, byte *ImgDst,
                           std::list<Contour<T>> contours_list) {
    int nc = 0;
    byte pixelMag; //pixel magnitude
    Contour<T> contour;
    PixelDirection traceDir;
    contourTrackState cState = INIT;
    Pixel<T> curr_pixel(start_pixel), next_pixel, first_pixel;

    traceDir.clockwise();
    contour.AddBoundaryPixel(curr_pixel);
    next_pixel = traceDir.getNeighbour(curr_pixel);
    while (nc < 8 && !IsPresentInActiveContours<T>(next_pixel, contours_list)) {
        //PIXEL_TRACE("visited pixel<start, current, next>", start_pixel, curr_pixel, next_pixel);
        if (IsPixelValid<T>(next_pixel, ImgWidth, ImgHeight)) {
            int index = next_pixel.getY() * ImgWidth + next_pixel.getX();
            pixelMag = (byte) Img[index]; 
            if (pixelMag > 0) {
                if (!contour.FindPixel(next_pixel)) {
                    nc = 0; // reset counter for new neigbourhood scan
                    contour.AddBoundaryPixel(next_pixel);
                    curr_pixel = next_pixel;
                    traceDir.reverseDirection();
                    if (cState == INIT)
                        first_pixel = next_pixel;
                } else if ((first_pixel == next_pixel) && (curr_pixel == start_pixel)) {
                    cState = DONE;
                    break;
                }
            }
        }
        traceDir.clockwise();
        next_pixel = traceDir.getNeighbour(curr_pixel);
        nc++;
    }

    if (contour.getSize()) {
        if (contour.getSize() > CONTOUR_THRESHOLD_SIZE) {
            //PIXEL_TRACE("detected contour<start, current, next>",
            //        start_pixel, curr_pixel, next_pixel);
             printf("detected contour size: %lu cState=%d\n",
                        contour.getSize(), cState);
        } else
            contour.ResetContour();
    }
    return contour;
}
#endif
