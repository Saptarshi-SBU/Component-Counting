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

#ifndef CCCONVEXHULL_HPP_
#define CCCONVEXHULL_HPP_

#include "CCPixel.hpp"
#include "CCPixelUtils.hpp"

template <class T>
static int PixelOrientation(const Pixel<T> &p1,
                            const Pixel<T> &p2,
                            const Pixel<T> &p3) {
    int val = (p2.getY() - p1.getY()) * (p3.getX() - p2.getX()) - 
              (p3.getY() - p2.getY()) * (p2.getX() - p1.getX());

    //std::cout << "orient:" << val << std::endl;

    if (val == 0)
        return 0;
    return val > 0 ? 1 : -1;
}

template<class T>
void MakeConvexHullUpper(std::list<Pixel<T>> pixels,
                         std::list<Pixel<T>> &upperhull) {
    std::stack<Pixel<int>> sp;

    pixels.sort();

    sp.push(pixels.back());
    pixels.pop_back();

    sp.push(pixels.back());
    pixels.pop_back();
   
    while (!pixels.empty() && sp.size() > 1) {
        Pixel<int> p3(pixels.back());
        pixels.pop_back();

        Pixel<int> p2(sp.top());
        sp.pop();

        Pixel<int> p1(sp.top());
        sp.pop();

        if (PixelOrientation(p1, p2, p3) < 0) {
            sp.push(p1);
            sp.push(p2);
            sp.push(p3);
        } else {
            while (sp.size()) {
                Pixel<int> p0(sp.top());
                sp.pop();
        
                if (PixelOrientation(p0, p1, p3) < 0) {
                    sp.push(p0);
                    break;
                } else {
                    p1 = p0;
                }
            }
            sp.push(p1);
            sp.push(p3);
        }
    }

    //std::cout << "stack size : " << sp.size() << std::endl;

    while (sp.size()) {
        Pixel<int> p(sp.top());
        upperhull.push_back(p);
        sp.pop();
        //std::cout << "HULL1 " << p.getX() << " " << p.getY() << std::endl;
    }
}

template<class T>
void MakeConvexHullLower(std::list<Pixel<T>> pixels,
                         std::list<Pixel<T>> &lowerhull) {
    std::stack<Pixel<int>> sp;

    pixels.sort();

    sp.push(pixels.back());
    pixels.pop_back();

    sp.push(pixels.back());
    pixels.pop_back();
   
    while (!pixels.empty() && sp.size() > 1) {
        Pixel<int> p3(pixels.back());
        pixels.pop_back();

        Pixel<int> p2(sp.top());
        sp.pop();

        Pixel<int> p1(sp.top());
        sp.pop();

        if (PixelOrientation(p1, p2, p3) > 0) {
            sp.push(p1);
            sp.push(p2);
            sp.push(p3);
        } else {
            while (sp.size()) {
                Pixel<int> p0(sp.top());
                sp.pop();

                if (PixelOrientation(p0, p1, p3) < 0) {
                    p1 = p0;
                } else {
                    sp.push(p0);
                    break;
                }
            }
            sp.push(p1);
            sp.push(p3);
        }
    }

    //std::cout << "stack size : " << sp.size() << std::endl;

    while (sp.size()) {
        Pixel<int> p(sp.top());
        lowerhull.push_back(p);
        sp.pop();
        //std::cout << "HULL2 " << p.getX() << " " << p.getY() << std::endl;
    }
}

template<class T>
std::list<Pixel<T>> MakeConvexHull(std::list<Pixel<T>> pixels) {
    std::list<Pixel<T>> upperhull, lowerhull, convexhull;

    MakeConvexHullUpper(pixels, upperhull);
    upperhull.pop_back();
    convexhull.splice(convexhull.begin(), upperhull);

    MakeConvexHullLower(pixels, lowerhull);
    lowerhull.reverse();
    if (lowerhull.size() > 1) {
        lowerhull.pop_back();
    }
    convexhull.splice(convexhull.end(), lowerhull);

    //std::cout << "size : " << convexhull.size() << std::endl;
    //for (auto &p : convexhull)
    //    std::cout << "convex hull " << p.getX() << ":" << p.getY() << std::endl;
    return convexhull;
}

template<class T>
void CalculateConvexHullGradient(std::list<Pixel<T>> pixels) {
    std::stack<Pixel<int>> sp;

    pixels.sort();

    sp.push(pixels.back());
    pixels.pop_back();

    sp.push(pixels.back());
    pixels.pop_back();

    while (!pixels.empty() && sp.size() > 1) {
        Pixel<int> p3(pixels.back());
        pixels.pop_back();

        Pixel<int> p2(sp.top());
        sp.pop();

        Pixel<int> p1(sp.top());
        sp.pop();

        //std::cout << "orientation : " << PixelOrientation(p1, p2, p3) << std::endl;
        sp.push(p2);
        sp.push(p3);
    }
}
#endif
