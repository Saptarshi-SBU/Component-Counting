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

#ifndef CCDOMINATINGPOINTS_HPP_
#define CCDOMINATINGPOINTS_HPP_

#include <map>
#include <list>
#include <vector>
#include <algorithm>
#include "CCPixel.hpp"
#include "CCLogger.hpp"

#define dbg_printf(...)

//#define dbg_printf printf

template<class T>
static int DominatingPoints(std::list<Pixel<T>> &polyPoints,
                            float dMin,
                            float dMax,
                            float alphaMax) {
    // vectors are easy to wrap
    std::map<int, float> candidates;
    std::vector<std::vector<float>> distMat;
    std::vector<Pixel<T>> points, dominatingPoints;

    int nr = polyPoints.size();
    distMat.resize(nr);

    for (auto &p : polyPoints)
        points.push_back(p);

    for (int i = 0; i < nr; i++) {
        Pixel<T> p(points[i]);
        distMat[i].resize(nr);
        for (int j = 0; j < nr; j++) {
            Pixel<T> q(points[j]);
            float d;
            if (i == j)
                continue;
            d = sqrt(std::pow(p.getX() - q.getX(), 2) + 
                     std::pow(p.getY() - q.getY(), 2));
            distMat[i][j] = d;
        }
    }

    for (int i = 0; i < nr; i++) {
        for (int j = 0; j < nr; j++)
            dbg_printf ("%f ", distMat[i][j]);
        dbg_printf("\n");
    }

    // 1st pass
    for (int i = 0; i < nr; i++) {

        for (int j = 0; j < nr; j++) {
            float d1;
            Pixel<T> p(points[j]);

            if (i == j)
                continue;

            d1 = distMat[i][j];
            CC_DEBUG("d1 :", d1, ":", dMin, ":", dMax);
            if (((d1 < dMin) || (d1 > dMax)))
                continue;

            for (int k = 0; k < nr; k++) {
                float d, d2, alpha;
                Pixel<T> q(points[k]);

                if ((j == k) || (i == k))
                    continue;

                d2 = distMat[i][k];
                CC_DEBUG("d2 :", d2, ":", dMin, ":", dMax);
                if (((d2 < dMin) || (d2 > dMax)))
                    continue;

                d = sqrt(std::pow(p.getX() - q.getX(), 2) + 
                         std::pow(p.getY() - q.getY(), 2));

                alpha = acos((std::pow(d1, 2) + std::pow(d2, 2) - std::pow(d, 2))
                    / (2.0 * distMat[i][j] * distMat[i][k])) * 180/M_PI;

                if (alpha > alphaMax)
                    continue;

                if ((candidates.find(i) == candidates.end()) ||
                    (candidates[i] < alpha))
                     candidates[i] = alpha;
            }
        }
    }

    // 2nd pass
    for (auto &c : candidates) {
        int i = c.first;
        float alpha = candidates[i];

        CC_DEBUG("alpha :", alpha, ":", alphaMax);
        
        for (int j = 0; j < distMat[i].size(); j++) {
            float d;

            if (i == j)
                continue;

            if (candidates.find(j) == candidates.end())
                continue;

            d = distMat[i][j];
            if ((d < dMin) || (d > dMax))
                continue;

            if (alpha < candidates[j]) {
                candidates[i] = 0; // suppress
                break;
            }
        }
    }

    for (auto &c : candidates) {
        if (c.second)
            dominatingPoints.push_back(points[c.first]);
    }

    CC_DEBUG("dominating points :", dominatingPoints.size());
    return 0;
}
#endif
