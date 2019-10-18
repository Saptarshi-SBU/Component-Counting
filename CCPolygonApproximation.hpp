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

#ifndef POLYAPPROX_HPP_
#define POLYAPPROX_HPP_

#include <list>
#include <algorithm>
#include "CCPixel.hpp"

#define dbg_printf(...)

//#define dbg_printf printf

#define PIXEL_TRACE2(s, a, b, c) \
    dbg_printf("Pixel Trace %s, p1:(%d, %d) p2:(%d, %d) p3:(%d, %d)\n", \
            s, (a).getX(), (a).getY(), (b).getX(), (b).getY(), (c).getX(), (c).getY())

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

    assert(list.size());
    for (auto &p : list) {
        if ((p == startPixel) || (p == endPixel))
            continue;
        PIXEL_TRACE2(__func__, startPixel, p, endPixel);
        float area = std::abs(
                startPixel.getX() * (p.getY()          - endPixel.getY()) +
                p.getX()          * (endPixel.getY()   - startPixel.getY()) +
                endPixel.getX()   * (startPixel.getY() - p.getY()))/2.0;
        float base = std::sqrt(
                std::pow(startPixel.getX() - endPixel.getX(), 2) +
                std::pow(startPixel.getY() - endPixel.getY(), 2));
        float height = (2.0 * area) / base;
        dbg_printf("%s : distance :%f\n", __func__, height);

        if (height > maxDist) {
            maxDist = height;
            vertexPixel = p;
        }
    }

    return maxDist;
}

// RDP Split-Phase
template<class T>
static int approxPolyDP(std::list<Pixel<T>> polyPoints,
        Pixel<T> startPixel,
        Pixel<T> endPixel,
        std::list<Pixel<T>> &polyPointsApprox,
        float distance) {
    float dMax;
    Pixel<T> vertexPixel;
    std::list<Pixel<T>> lpolyPoints, rpolyPoints;

    if (polyPoints.empty())
        goto exit_approx;

    dMax = FindPixelwithMaxDistance(polyPoints, startPixel, endPixel, vertexPixel);
    if (dMax < distance) {
        dbg_printf("%s : distance :%f\n", __func__, distance);
        goto exit_approx;
    }

    dbg_printf("ppn distance :%f-%f (%d, %d)\n",
            dMax, distance, vertexPixel.getX(), vertexPixel.getY());

    // split segments
    lpolyPoints = PixelsGroupBy(polyPoints, vertexPixel, ComparatorLesserPixel<T>());
    polyPointsApprox.push_back(vertexPixel);
    rpolyPoints = PixelsGroupBy(polyPoints, vertexPixel, ComparatorGreaterPixel<T>());

    return 1 +
        approxPolyDP(lpolyPoints, startPixel, vertexPixel, polyPointsApprox, distance) +
        approxPolyDP(rpolyPoints, endPixel, vertexPixel, polyPointsApprox, distance);

exit_approx:
    return 0;
}

// RDP Merge-Phase
template<class T>
static int MergePoints(std::list<Pixel<T>> &polyPoints, float distance) {
    float dMax;
    Pixel<T> lastp;
    std::stack<Pixel<T>> sp;
    std::list<Pixel<T>> removePixels, tmpPixels;
    if (polyPoints.empty())
        goto exit_approx;

    for (auto &p : polyPoints) {
        CC_INFO("SPLITDATAPOINTS", p.getX(), p.getY());
        sp.push(p);
    }

    lastp = sp.top();

    while (sp.size() > 2) {
        Pixel<T> vertexPixel;
        std::list<Pixel<T>> polyPoints2;

        Pixel<T> p3(sp.top());
        sp.pop();

        Pixel<T> p2(sp.top());
        sp.pop();

        Pixel<T> p1(sp.top());
        sp.pop();

        polyPoints2.push_back(p2);
        dMax = FindPixelwithMaxDistance(polyPoints2, p3, p1, vertexPixel);
        polyPoints2.clear();
        if (dMax < distance) {
            sp.push(p1);
            sp.push(p3);
            removePixels.push_back(p2);
            continue;
        }
        sp.push(p1);
        sp.push(p2);
    }

    if (sp.size() == 2) {
        Pixel<T> vertexPixel;
        std::list<Pixel<T>> polyPoints2;

        Pixel<T> p3(sp.top());
        sp.pop();

        Pixel<T> p2(sp.top());
        sp.pop();

        Pixel<T> p1(lastp);

        polyPoints2.push_back(p2);
        dMax = FindPixelwithMaxDistance(polyPoints2, p3, p1, vertexPixel);
        polyPoints2.clear();
        if (dMax < distance) {
            removePixels.push_back(p2);
        }
    }

    for (auto &p : polyPoints) {
        if (std::find(removePixels.begin(), removePixels.end(), p) == removePixels.end())
            tmpPixels.push_back(p);
    }

    polyPoints = tmpPixels;

exit_approx:
    return 0;
}
#endif
