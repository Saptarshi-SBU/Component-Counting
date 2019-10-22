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

#include "CCUUid.hpp"
#include "CCPixel.hpp"
#include "CCConvexHull.hpp"
#include "CCPolygonApproximation.hpp"
#include "CCImageReader.hpp"

typedef unsigned char byte;

#define dbg_printf(...)

template<class T>
struct Contour {
    enum ContourType {
        HOLE,

        OUTER,
    };

    CCUUid _uuid;

    Pixel<T> _start;

    ContourType _type;

    std::list<Pixel<T>> boundaryPixels;

    Contour() : _uuid() {}

    Contour(const Contour<T> &contour) {
        _uuid  = CCUUid();
        _start = contour._start;
        _type  = contour._type;
        boundaryPixels = contour.boundaryPixels;
    }

    Contour(const Pixel<T>& pixel, ContourType type) :
        _uuid(), _start(pixel), _type(type) {}

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
        return (std::find(boundaryPixels.begin(), boundaryPixels.end(), pixel)
            != boundaryPixels.end());
    }

    bool empty(void) const noexcept {
        return boundaryPixels.empty();
    }

    std::string getUUId(void) {
        return _uuid.getString();
    }

    const size_t getSize(void) const noexcept {
        return boundaryPixels.size();
    }

    const size_t getLength(void) const noexcept {
        size_t sum = 0;
        std::stack<Pixel<T>> sp;
        for (auto &p : boundaryPixels)
            sp.push(p);

        Pixel<T> p1(sp.top());
        sp.pop();

        while (!sp.empty()) {
            Pixel<T> p2(sp.top());
            sp.pop();

            sum += sqrt(std::pow(p2.getX() - p1.getX(), 2) +
                        std::pow(p2.getY() - p1.getY(), 2));
        }
        return sum;
    }

    void ResetContour(void) {
        boundaryPixels.clear();
    }

    void makeConvexHull(void) {
        for (auto &p : boundaryPixels) 
            CC_INFO("BorderPixels", p.getX(), p.getY());

        boundaryPixels = MakeConvexHull<T>(boundaryPixels);
        for (auto &p : boundaryPixels) 
            CC_INFO("HullPixels", p.getX(), p.getY());
    }

    void ApproxPoly(int distance) {
        size_t size = getLength();
        CC_INFO("contour perimeter", size, 0.01 * size);
        std::list<Pixel<int>> polyPointsApprox, polyPoints;

        polyPointsApprox.push_back(*boundaryPixels.begin());

        approxPolyDP(boundaryPixels,
                    *boundaryPixels.begin(),
                    *boundaryPixels.rbegin(),
                     polyPointsApprox,
                     distance);

        polyPointsApprox.push_back(*boundaryPixels.rbegin());

        for (auto &p : boundaryPixels) {
            if (std::find(polyPointsApprox.begin(), polyPointsApprox.end(), p)
                    != polyPointsApprox.end()) {
                polyPoints.push_back(p);
            }
        }

        CC_INFO("polypoints (split-phase) :", polyPoints.size());
        MergePoints<int>(polyPoints, 10);
        MergePoints<int>(polyPoints, 10);
        boundaryPixels = polyPoints;
        for (auto &p : boundaryPixels) 
            CC_INFO("MergedBorderPixels", p.getX(), p.getY());
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
        int i = 0, p = 0, q = 1, n;

        assert(boundaryPixels.size());
        // use array indexed container
        std::vector<Pixel<float>> pixelVec;
        for (auto &p : boundaryPixels)
            pixelVec.push_back(Pixel<float>(p.getX(), p.getY()));

        n = pixelVec.size();
        if (n < 3)
            return;

        while (i < n) {
            std::list<Pixel<float>> newPoints;
            Pixel<float> p1(pixelVec.at(p)), p2(pixelVec.at(q));

            lineGeneration<float>(p1, p2, newPoints);

            for (auto &p : newPoints) {
                int index;
                Pixel<float> pixel(p);
                index = (int)pixel.getY() * ImgWidth + (int)pixel.getX();
                Img[index] = 255;
            }
            newPoints.clear();
            p = (p + 1) % n;
            q = (q + 1) % n;
            i++;
        }
    }
};

#endif
