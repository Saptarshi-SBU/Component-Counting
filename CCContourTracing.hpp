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

#ifndef CCONTOURTRACING_HPP_
#define CCONTOURTRACING_HPP_

#include <list>
#include "CCPixel.hpp"
#include "CCPixelUtils.hpp"
#include "CCContour.hpp"
#include "CCImageReader.hpp"

#define CONTOUR_THRESHOLD_SIZE 2

#define dbg2_printf(...)

//#define dbg2_printf printf

#define PIXEL_TRACE(s, a, b, c) \
        dbg2_printf("Pixel Trace %s, p: %d %d q: %d %d r: %d,%d\n", \
                s, (a).getX(), (a).getY(), (b).getX(), (b).getY(), (c).getX(), (c).getY())

constexpr int PixelDirection::dirx[];

constexpr int PixelDirection::diry[];

enum contourTrackState {
    INIT,
    DONE,
};

template<class T>
static bool IsPixelVisited(const Pixel<T> &pixel, const std::list<Contour<T>> &contours_list) {
    for (auto &c : contours_list) {
        if (!c.empty() && c.FindPixel(pixel))
            return true;
    }
    return false;
}

template<class T>
static bool
IsBorderPixel(byte *Img, const Pixel<T> &start_pixel, int ImgHeight, int ImgWidth) {
        Contour<T> contour;
        PixelDirection traceDir;
        Pixel<T> neighbour_pixel, np(start_pixel);
        PixelDirection::Direction entry_dir;
        int index, count = 0;

        if (!IsPixelValid<T>(np, ImgWidth, ImgHeight))
            return false;

        index = np.getY() * ImgWidth + np.getX();
        if (!Img[index])
            return false;

        traceDir.clockwise();
        entry_dir = traceDir.getDirection();
        do {
                neighbour_pixel = traceDir.getNeighbour(start_pixel);
                if (neighbour_pixel.valid()) {
                        index = neighbour_pixel.getY() * ImgWidth + neighbour_pixel.getX();
                        if (Img[index])
                                count++;
                        //PIXEL_TRACE("bp pixel<start, current, next>",
                        //    start_pixel, start_pixel, neighbour_pixel);
                }
                traceDir.clockwise();
        } while (entry_dir != traceDir.getDirection());

        //dbg2_printf("border pixel (%d, %d) count=%d\n",
        //        start_pixel.getX(), start_pixel.getY(), count);
        return count < 8;
}

//core routine
template<class T>
static Contour<T>
BorderFollowingStrategy(byte *Img, 
                           const Pixel<T> &start_pixel,
                           int ImgHeight, int ImgWidth, byte *ImgDst,
                           std::list<Contour<T>> contours_list) {
    int nc = 0, index;
    Contour<T> contour;
    PixelDirection traceDir;
    Pixel<T> curr_pixel(start_pixel), next_pixel, first_pixel;

    if (!IsBorderPixel<T>(Img, curr_pixel, ImgHeight, ImgWidth))
        return contour;

    traceDir.clockwise();
    contour.AddBoundaryPixel(curr_pixel);
    next_pixel = traceDir.getNeighbour(curr_pixel);
    PIXEL_TRACE("start pixel<start, current, next>",
        start_pixel, curr_pixel, start_pixel);
    // follow border pixels
    //while (nc < 8 && !IsPixelVisited<T>(next_pixel, contours_list)) {
    while (nc < 8) {
        if (!next_pixel.valid())
            goto nextbp;
        index = next_pixel.getY() * ImgWidth + next_pixel.getX();
        if ((byte) Img[index]) {
            if (contour.FindPixel(next_pixel))
                break;
            if (!IsBorderPixel<T>(Img, next_pixel, ImgHeight, ImgWidth))
                goto nextbp;
            // reset counter for new neigbourhood scan
            nc = 0;
            contour.AddBoundaryPixel(next_pixel);
            PIXEL_TRACE("visited pixel<start, current, next>",
                start_pixel, curr_pixel, next_pixel);
            curr_pixel = next_pixel;
            traceDir.reverseDirection();
        }

    nextbp:
        traceDir.clockwise();
        next_pixel = traceDir.getNeighbour(curr_pixel);
        nc++;
    }

    if (contour.getSize() > CONTOUR_THRESHOLD_SIZE) {
        CC_DEBUG("detected contour size: %lu\n", contour.getSize());
        PIXEL_TRACE("end pixel<start, current, next>",
            start_pixel, curr_pixel, next_pixel);
    } else
            contour.ResetContour();

    return contour;
}
#endif
