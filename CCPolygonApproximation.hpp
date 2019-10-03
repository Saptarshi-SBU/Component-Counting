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

#ifndef CONTOUR_HPP_
#define CONTOUR_HPP_

#include <list>
#include <algorithm>
#include "CCPixel.hpp"

#define dbg_printf(...)

//#define dbg_printf printf

#define PIXEL_TRACE2(s, a, b) \
    dbg_printf("Pixel Trace %s, p:(%d, %d) q:(%d, %d)\n", \
            s, (a).getX(), (a).getY(), (b).getX(), (b).getY())

template <class T>
struct ComparatorLesserPixel {
    bool operator() (Pixel<T> const &a, Pixel<T> const &b) {
        if (a.getX() != b.getX())
            return a.getX() < b.getX();
        else
            return a.getY() < b.getY();
    }
};

template <class T>
struct ComparatorGreaterPixel {
    bool operator() (const Pixel<T> &a, const Pixel<T> &b) {
        if (a.getX() != b.getX())
            return a.getX() > b.getX();
        else
            return a.getY() > b.getY();
    }
};

template <class T, class Comp>
std::list<Pixel<T>> PixelsGroupBy(std::list<Pixel<T>> &plist,
    const Pixel<T> &vertexPixel, Comp comp) {
    std::list<Pixel<T>> pixelList;
    plist.sort(comp);
    auto iter = std::find(plist.begin(), plist.end(), vertexPixel);
    for (auto i = plist.begin() ; i != iter; i++)
        pixelList.push_back(*i);
    return pixelList;
}

template<class T>
float FindPixelwithMaxDistance(const std::list<Pixel<T>> &list,
        Pixel<T> startPixel,
        Pixel<T> endPixel,
        Pixel<T> &vertexPixel) {
    size_t maxDist = 0;

    PIXEL_TRACE2(__func__, startPixel, endPixel);
    for (auto &p : list) {
        if ((p == startPixel) || (p == endPixel))
            continue;
        PIXEL_TRACE2(__func__, startPixel, p);
        float area = std::abs(
                startPixel.getX() * (p.getY()          - endPixel.getY()) +
                p.getX()          * (endPixel.getY()   - startPixel.getY()) +
                endPixel.getX()   * (startPixel.getY() - p.getY()))/2.0;
        float base = std::sqrt(
                std::pow(startPixel.getX() - endPixel.getX(), 2) +
                std::pow(startPixel.getY() - endPixel.getY(), 2));
        float height = (2.0 * area) / base;

        if (height > maxDist) {
            maxDist = height;
            vertexPixel = p;
            dbg_printf("curr max area :%f (%d, %d)\n", area, p.getX(), p.getY());
        }
    }

    return maxDist;
}

// RDP algorithm :
// https://en.wikipedia.org/wiki/Ramer%E2%80%93Douglas%E2%80%93Peucker_algorithm
template<class T>
static int approxPolyDP(std::list<Pixel<T>> polyPoints,
        Pixel<T> startPixel,
        Pixel<T> endPixel,
        std::list<Pixel<T>> &polyPointsApprox,
        float distance) {
    float dMax;
    Pixel<T> vertexPixel;
    std::list<Pixel<T>> lpolyPoints, rpolyPoints;

    // base case
    if (polyPoints.empty())
        goto exit_approx;

    // sort based on x-axis
    polyPoints.sort(ComparatorLesserPixel<T>());

    // identify start and end segments, and get max perpendicular distance
    // for rest of the points in the contour
    dMax = FindPixelwithMaxDistance(polyPoints, startPixel, endPixel, vertexPixel);
    if (dMax < distance)
        goto exit_approx;

    dbg_printf("distance :%f\n", distance);
    printf("ppn distance :%f-%f (%d, %d)\n",
            dMax, distance, vertexPixel.getX(), vertexPixel.getY());

    polyPointsApprox.push_back(vertexPixel);
    // partition the poly into two segments
    lpolyPoints = PixelsGroupBy(polyPoints, vertexPixel, ComparatorLesserPixel<T>());
    rpolyPoints = PixelsGroupBy(polyPoints, vertexPixel, ComparatorGreaterPixel<T>());

    // aproximate num polyPointsApprox
    return 1 +
        approxPolyDP(lpolyPoints, startPixel, vertexPixel, polyPointsApprox, distance) +
        approxPolyDP(rpolyPoints, endPixel, vertexPixel, polyPointsApprox, distance);

exit_approx:
    return 0;
}

#endif
