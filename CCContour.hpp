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

#ifndef CCONTOUR_HPP_
#define CCONTOUR_HPP_

#include <set>
#include <list>

#include "CCPixel.hpp"
#include "CCImageReader.hpp"

typedef unsigned char byte;

#define dbg_printf(...)

template<class T>
struct Contour {
    enum ContourType {
        HOLE,

        OUTER,
    };

    Pixel<T> _start;

    ContourType _type;

    std::list<Pixel<T>> boundaryPixels;

    std::list<Pixel<T>> boundaryPixelsApprox;

    Contour() {}

    Contour(const Contour<T> &contour) {
        _start = contour._start;
        _type  = contour._type;
        boundaryPixels = contour.boundaryPixels;
        boundaryPixelsApprox = contour.boundaryPixelsApprox;
    }

    Contour(const Pixel<T>& pixel, ContourType type) : _start(pixel), _type(type) {}

    ~Contour() {}

    void SetStart(const Pixel<T> &pixel) {
        _start = pixel;
    }

    void SetType(ContourType type) {
        _type = type;
    }

    void AddBoundaryPixel(const Pixel<T> &pixel) {
        boundaryPixels.push_back(pixel);
    }

    bool FindPixel(const Pixel<T> &pixel) const noexcept {
        return (std::find(boundaryPixels.begin(), boundaryPixels.end(), pixel) != boundaryPixels.end());
    }

    bool empty(void) const noexcept {
        return boundaryPixels.empty();
    }

    const size_t getSize(void) const noexcept {
        return boundaryPixels.size();
    }

    void ResetContour(void) {
        boundaryPixels.clear();
        boundaryPixelsApprox.clear();
    }

    void drawContour(unsigned char *Img, int ImgWidth, int ImgHeight) {
        for (auto &p : boundaryPixels) {
            int index;
            Pixel<T> pixel(p);
            index = pixel.getY() * ImgWidth + pixel.getX();
            Img[index] = 255;
        }
    }

    void drawContourApprox(unsigned char *Img, int ImgWidth, int ImgHeight) {
        for (auto &p : boundaryPixelsApprox) {
            int index;
            Pixel<T> pixel(p);
            index = pixel.getY() * ImgWidth + pixel.getX();
            Img[index] = 255;
        }
    }

    void drawContourApprox2(unsigned char *Img, int ImgWidth, int ImgHeight) {
        std::vector<Pixel<float>> pixelVec;

        assert(boundaryPixelsApprox.size());

        for (auto &p : boundaryPixelsApprox) {
            pixelVec.push_back(Pixel<float>(p.getX(), p.getY()));
            printf("x=%d, y=%d\n", p.getX(), p.getY());
        }

        int p = 0, q = 1;
        int i = 0, count = pixelVec.size();
        while (i < count) {
            std::list<Pixel<float>> points;
            Pixel<float> p1(pixelVec.at(p)), p2(pixelVec.at(q));
            printf("COUNT :%lu, count=%d\n", pixelVec.size(), i);
            lineGeneration<float>(p1, p2, points);
            for (auto &p : points) {
                int index;
                Pixel<float> pixel(p);
                index = (int)pixel.getY() * ImgWidth + (int)pixel.getX();
                Img[index] = 255;
            }
            points.clear();
            p = (p + 1) % count;
            q = (q + 1) % count;
            i++;
        }
    }
};

#endif
