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

#ifndef CCPIXEL_HPP_
#define CCPIXEL_HPP_

#include <list>
#include <algorithm>

// forward declaration
template<class T>
struct Pixel;

template<class T>
static unsigned long convertULong(T x) {
        unsigned long ulx = static_cast<unsigned long>(x);
        return ulx;
}

template<class T>
struct Pixel {

        T _x;

        T _y;

        Pixel() : _x(0), _y(0) {}

        Pixel(T x) : _x(x), _y(0) {}

        explicit Pixel(T x, T y) : _x(x), _y(y) {}

        Pixel(const Pixel<T> &pixel) {
                _x = pixel._x;
                _y = pixel._y;
        }

        Pixel<T>& operator=(const Pixel<T> &pixel) {
                _x = pixel._x;
                _y = pixel._y;
                return *this;
        }

        Pixel<T> operator+(const Pixel<T> &pixel) {
                Pixel<T> result;
                result._x = _x + pixel.getX();
                result._y = _y + pixel.getY();
                return result;
        }

        const Pixel<T> operator-(Pixel<T> &pixel) const {
                Pixel<T> result;
                result._x = _x - pixel.getX();
                result._y = _y - pixel.getY();
                return result;
        }

        friend bool operator==(const Pixel<T> &pixel_a, const Pixel<T> &pixel_b) {
                return ((pixel_a.getX() == pixel_b.getX()) &&
                    pixel_a.getY() == pixel_b.getY());
        }

        friend bool operator!=(const Pixel<T> &pixel_a, const Pixel<T> &pixel_b) {
                return ((pixel_a.getX() != pixel_b.getX()) ||
                    pixel_a.getY() != pixel_b.getY());
        }

        friend bool operator<(const Pixel<T> &pixel_a, const Pixel<T> &pixel_b) {
                if (pixel_a.getX() == pixel_b.getX())
                    return pixel_a.getY() < pixel_b.getY();
                else
                    return pixel_a.getX() < pixel_b.getX();
        }

        friend bool operator>(const Pixel<T> &pixel_a, const Pixel<T> &pixel_b) {
                if (pixel_a.getX() == pixel_b.getX())
                    return pixel_a.getY() > pixel_b.getY();
                else
                    return pixel_a.getX() > pixel_b.getX();
        }

        T getX(void) const noexcept {
                return _x;
        }

        T getY(void) const noexcept {
                return _y;
        }

        void setX(T x) {
                _x = x;
        }
                
        void setY(T y) {
                _y = y;
        }

        bool valid(void) const noexcept {
                return (_x >= 0) && (_y >= 0);
        }
};

#endif
