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

#ifndef CCPIXELUTILS_HPP_
#define CCPIXELUTILS_HPP_

#include <list>
#include <algorithm>

#define dbg_printf(...)

//#define dbg_printf printf

#define PIXEL_TRACE3(s, a, b) \
        dbg_printf("Pixel Trace %s, p:(%f, %f) q:(%f, %f)\n", \
                s, (a).getX(), (a).getY(), (b).getX(), (b).getY())

template<class T>
bool IsPixelValid(Pixel<T> &pixel, int width, int height) {
    return (pixel.valid() && (pixel.getX() < width) && (pixel.getY() < height));
}

struct PixelDirection {

    static const int MAX_NEIGHBOURHOOD = 8;

    enum Direction {
        NORTH, NORTH_EAST, EAST, SOUTH_EAST, SOUTH, SOUTH_WEST, WEST, NORTH_WEST
    };

    //
    // In C++11, non-static data members, static constexpr data members, and
    // static const data members of integral or enumeration type may be
    // initialized in the class declaration.
    // Since float and double are not of integral or enumeration type, such
    // members must either be constexpr, or non-static in order for the
    // initializer in the class definition to be permitted.
    //
    static constexpr int dirx[MAX_NEIGHBOURHOOD] = { 0, 1, 1, 1, 0, -1, -1, -1 };

    static constexpr int diry[MAX_NEIGHBOURHOOD] = { -1, -1, 0, 1, 1, 1, 0, -1 };  

    Direction dir;

    PixelDirection() : dir(NORTH_WEST) {}

    Direction clockwise(void) noexcept {
        dir = static_cast<Direction>((dir + 1) % MAX_NEIGHBOURHOOD);
        return dir;
    }

    Direction counter_clockwise(void) noexcept {
        dir = static_cast<Direction>((dir - 1) < 0 ? (dir - 1) : NORTH_WEST);
        return dir;
    }

    Direction getDirection(void) const noexcept {
        return dir;
    }

    std::string printDirection(void) const noexcept {
        switch (dir) {
        case NORTH:
            return std::string("NORTH");
            break;
        case NORTH_EAST:
            return std::string("NORTH_EAST");
            break;
        case EAST:
            return std::string("EAST");
            break;
        case SOUTH_EAST:
            return std::string("SOUTH_EAST");
            break;
        case SOUTH:
            return std::string("SOUTH");
            break;
        case SOUTH_WEST:
            return std::string("SOUTH_WEST");
            break;
        case WEST:
            return std::string("WEST");
            break;
        case NORTH_WEST:
            return std::string("NORTH_WEST");
            break;
        default:
            assert(0);
        }
    }

    void resetDirection(void) noexcept {
        dir = NORTH_WEST;
    }

    void reverseDirection(void) noexcept {
        dir = static_cast<Direction>((dir + 4) % MAX_NEIGHBOURHOOD);
    }

    template<class T> // template method
    Pixel<T> getNeighbour(const Pixel<T> &pixel) {
            Pixel<T> newpixel(pixel);
            newpixel.setX(newpixel.getX() + dirx[dir]);
            newpixel.setY(newpixel.getY() + diry[dir]);
            return newpixel;
    }
};

// DDA algorithm
template<class T>
static void lineGeneration(Pixel<T> p, Pixel<T> q, std::list<Pixel<T>> &points) {
        T x, y;
        float slope, dx, dy;
        Pixel<T> pixel;

        if (p > q)
                std::swap(p, q);

        x = p.getX();
        y = p.getY();
        dx = q.getX() - p.getX(), dy = q.getY() - p.getY(); 

        assert(dx);
        slope = dy/dx;
        PIXEL_TRACE3("points :", p, q);

        points.push_back(p);
        do {
                if (std::abs(slope) <= 1) {
                        y += slope;
                        x += 1;
                        dbg_printf("x=%f y=%f\n", x, y);
                        if (x >= q.getX())
                                break;
                } else {
                        x += 1.0/slope;
                        y += 1;
                        dbg_printf("x=%f y=%f\n", x, y);
                        if (y >= q.getY())
                                break;
                }

                pixel = Pixel<T>(round(x), round(y));
                points.push_back(pixel);
                PIXEL_TRACE3("line :", p, pixel);
        } while (true);

        points.push_back(q);
}
#endif
